/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/06 14:16:34 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

// Get path from the env variable. 
// If envv is (null) set the path to linux defaults
char	**get_path(char **envv)
{
	char	**env_p;
	char	*env;

	env = ft_getenv(envv, "PATH");
	if (env == NULL)
	{
		free(env);
		env = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	}
	env_p = ft_split(env, ':');
	return (env_p);
}

int	arg_check(int ac, char **av)
{
	if (ac != 1 && ac != 3)
	{
		printf("%s\n", USAGE_MSG);
		return (0);
	}
	if (ac == 3 && (ft_strlen(av[1]) != 2 || ft_strncmp(av[1], "-c", 2)))
	{
		printf("%s\n", USAGE_MSG);
		return (0);
	}
	return (1);
}

void	print_token(t_token **root)
{
	t_token *current;
	t_token	*tmp;

	current = *root;
	while (current != NULL)
	{
		// printf("token type %i | value %s\n", current->token_type, current->value);
		// free(current->value);
		// if (current->prev != NULL)
		// 	printf("token prev %i | value %s\n", current->prev->token_type, current->prev->value);
		tmp = current;
		current = current->next;
		// free(tmp);
	}
	// free(current);
	*root = NULL;
}
void	free_token(t_data *data)
{
	t_token	*current;
	t_token	*tmp;

	if (*data->token_root == NULL)
		return ;
	current = *data->token_root;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
	*data->token_root = NULL;
}

void	free_cmd_list(t_data *data)
{
	t_cmd	*current;
	t_cmd	*tmp;

	if (*data->cmd_list == NULL)
		return ;
	current = *data->cmd_list;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		// if (tmp->cmd)
		// 	free(tmp->cmd);
		if (tmp->args)
			ft_free_tab(tmp->args);
		// if (tmp->pipe_fd)
		// 	free(tmp->pipe_fd);
		free(tmp);
	}
	*data->cmd_list = NULL;
}

int	free_data(t_data *data)
{
	free_token(data);
	free_cmd_list(data);
	free(data->user_input);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envv)
{
	t_data	data;
	char	*input;
	pid_t	pid;		
	t_token **tmp;
	t_cmd	*tmp_cmd;
	int		status;
	int		exit_status;

	ft_memset(&data, '0', sizeof(data));
	data.token_root = (t_token **)ft_calloc(1, sizeof(t_token *));
	tmp = data.token_root;
	data.cmd_list = (t_cmd **)ft_calloc(1, sizeof(t_cmd *));
	*data.cmd_list = NULL;
	data.parse_status = NONE;
	data.envv = envv;
	*tmp = NULL;
	t_cmd *cmd = *data.cmd_list;
	if (!arg_check(ac, av))
		return (EXIT_FAILURE);
	input = NULL;
	while(1)
	{
		signals_interact();
		// data.user_input = readline("$ ");
		ft_printf("%s$ ", ft_getenv(envv, "PWD"));
		input = get_next_line(0);
		data.user_input = ft_substr(input, 0, ft_strlen(input) - 1);
		free(input);
		ft_printf("userinput %s\n", data.user_input);
		signals_no_interact();
		if (data.user_input != NULL && !strcmp(data.user_input, "exit"))
			break ;
		scan_input(&data);
		parse_token(&data);
		build_cmd_list(&data, *data.token_root);
		cmd = *data.cmd_list;
		if (cmd)
		{
			while(data.pid != 0 && cmd) 
			{
				data.pid = fork();
				if (data.pid == 0)
				{
					exec_cmd(cmd, &data);
					free_data(&data);
					exit (1);
				}
				cmd = cmd->next;
			}
			int	wpid = 0;
			cmd = *data.cmd_list;
			while(wpid != -1 || errno != ECHILD) 
			{
				close_pipes(data.cmd_list, NULL);
				wpid = waitpid(-1, &status, 0);
				if (wpid == data.pid)
					exit_status = status;
				continue ;
			}
			// tmp_cmd = cmd;
			// cmd = cmd->next;
			// free(tmp_cmd);
		}
		// free(cmd);
		// *data.cmd_list = NULL;
		free_data(&data);
		// if (data.parse_status == ODQUOTE)
		// 		printf("still need to close the dquotes mate...\n");
		// else if (data.parse_status == OSQUOTE)
		// 		printf("still need to close the squotes mate...\n");
		// else 
		// {
		// 	print_token(data.token_root);
		// }
	}
	free_data(&data);
	free(data.token_root);
	free(data.cmd_list);
	return (0);
}
