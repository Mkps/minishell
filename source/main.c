/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/08/04 09:04:04 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
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
		printf("token type %i | value %s\n", current->token_type, current->value);
		free(current->value);
		tmp = current;
		current = current->next;
		free(tmp);
	}
	free(current);
	*root = NULL;
}

int	main(int ac, char **av, char **envv)
{
	t_data	data;
	char	*input;
	pid_t	pid;		
	t_token **tmp;

	ft_memset(&data, '0', sizeof(data));
	data.token_root = (t_token **)ft_calloc(1, sizeof(t_token *));
	tmp = data.token_root;
	data.parse_status = NONE;
	data.envv = envv;
	*tmp = NULL;
	if (!arg_check(ac, av))
		return (EXIT_FAILURE);
	input = NULL;
	while(1)
	{
		signals_interact();
		data.user_input = readline("$ ");
		signals_no_interact();
		if (data.user_input == 0 || !strcmp(data.user_input, "exit"))
			break ;
		scan_input(&data);
		pid = fork();
		if (pid == 0)
		{
			exec_cmd(data.user_input,envv);
			free(data.user_input);
			exit (1);
		}
		waitpid(pid, NULL, 0);
		free(input);
		if (data.parse_status == ODQUOTE)
				printf("still need to close the dquotes mate...\n");
		else if (data.parse_status == OSQUOTE)
				printf("still need to close the squotes mate...\n");
		else 
		{
			print_token(data.token_root);
		}
	}
	free(input);
	return (0);
}
