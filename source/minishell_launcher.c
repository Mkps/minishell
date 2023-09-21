/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_launcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:21:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/19 22:58:06 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

void	minishell_inline(t_data *data, char *user_input)
{
	char	**cmd_list;
	int		i;

	if (user_input)
	{
		cmd_list = ft_split(user_input, ';');
		data->raw_input = ft_strdup(user_input);
	}
	else
	{
		cmd_list = NULL;	
		data->raw_input = NULL;
	}
	i = -1; 
	while (cmd_list[++i])
	{
		data->user_input = cmd_list[i];
		if (i > 0)
			data->raw_input = NULL;
		scan_input(data);
		if (check_error(data) == EXIT_SUCCESS)
		{
			parse_token(data);
			parse_near_quote(data);
			build_cmd_list(data, *data->token_root);
			execute(data);
		}
		free_data(data);
	}
	free(cmd_list);
	exit(g_exit_code);
}
void	minishell_subshell(t_data *data, char *user_input)
{
	t_data	new_data;
	int		i;

	init_data(&new_data);
	import_envv(&new_data, data->envv);
	new_data.user_input = user_input;
	new_data.raw_input = NULL;
	new_data.cmd_split = ft_split(new_data.user_input, ';');
	free_data(data);
	i = -1; 
	while (new_data.cmd_split[++i])
	{
		new_data.user_input = ft_strdup(new_data.cmd_split[i]);
		scan_input(&new_data);
		if (check_error(&new_data) == EXIT_SUCCESS)
		{
			parse_token(&new_data);
			parse_near_quote(&new_data);
			build_cmd_list(&new_data, *new_data.token_root);
			if (init_io_redir(&new_data) == EXIT_SUCCESS)
				execute(&new_data);
		}
		free_data(&new_data);
		dup2(new_data.old_fd[0], 0);
	}
	ft_free_tab(new_data.cmd_split);
	exit (g_exit_code);
}

char	*glob_home(t_data *data, char *str)
{
	char	*home;
	char	*ret;
	int		i;

	home = get_var(data, "HOME");
	i = 0;
	while (str[i] && home[i] && str[i] == home[i])
		i++;
	if (i > 0)
	{
		ret = ft_strdup(str + (i - 1));
		ret[0] = '~';
		return(ret);
	}
	return (ft_strdup(str));
}
char	*get_session(t_data *data)
{
	char	*tmp;
	char	*ret;
	int		s_idx;
	int		e_idx;

	if ((tmp = get_var(data, "SESSION_MANAGER")) != NULL)
	{
		s_idx = 0;
		while (tmp[s_idx] && tmp[s_idx] != '/')
			s_idx++;
		s_idx++;
		e_idx = s_idx;
		while (tmp[e_idx] && tmp[e_idx] != ':' && tmp[e_idx] != '.')
			e_idx++;
		ret = ft_strdup(&tmp[s_idx]);
		ret = ft_str_extract_free(ret, (e_idx - s_idx));
		return (ret);
	}	
	else
		return (ft_strdup("localhost"));
}
char	*set_prompt(t_data *data)
{
	char	*prompt;
  
	prompt = ft_strappend(GREEN, get_var(data, "USER"), 0);
	prompt = ft_strappend(prompt, "@", 2);
	prompt = ft_strappend(prompt, get_session(data), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, ":", 2);
	prompt = ft_strappend(prompt, CYAN, 2);
	prompt = ft_strappend(prompt, glob_home(data, get_var(data, "PWD")), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, "\n$ ", 2);
	return (prompt);
}

void	prompt_user(t_data *data)
{
	char	*prompt;

	prompt = set_prompt(data);
	signal(SIGINT, (void (*) (int))redisplay_prompt);
	redisplay_prompt(42, prompt);
	data->user_input = NULL;
	data->raw_input = NULL;
	data->user_input = readline(prompt);
	free(prompt);
	if ((data->user_input != NULL && (!strcmp(data->user_input, "exit")) || data->user_input == NULL))
	{
		if (!data->user_input)
			write(1, "exit\n", 5);
		exit(0);
	}
}

int		ft_get_token_err(char *input, t_data *data);
int		check_err(char *input, t_data *data)
{
	int	i;
	int	input_length;

	input = data->user_input;
	data->parse_status = NONE;
	i = 0;
	if (*input == '#')
	{
		add_history(data->raw_input);
		return (EXIT_FAILURE);
	}
	while (*input && ft_is_ws(*input)) input++;
	if (input == NULL || *input == 0 || *input == '#')
		return (EXIT_FAILURE);
	if (data->raw_input)
	{
		add_history(data->raw_input);
		free(data->raw_input);
		data->raw_input = NULL;
	}
	input_length = ft_strlen(input);
	while(i <= input_length)
		i += ft_get_token_err(input + i, data); 
	return (check_error(data));
}
int		check_error_raw(t_data *data)
{
	int	i;
	char	*tmp;

	i = -1;
	tmp = ft_strdup(data->raw_input);
	data->raw_input = ft_strdup(data->user_input);
	if (check_err(data->raw_input, data))
	{
		free(tmp);
		free_data(data);
		ft_free_tab(data->cmd_split);
		return (1);
	}
	else
	{
		data->raw_input = ft_strdup(data->user_input);
		free_token(data);
		free(data->user_input);
	}
	while (data->cmd_split[++i])
	{
		data->user_input = ft_strdup(data->cmd_split[i]);
		scan_input(data);
		if (check_error(data) != EXIT_SUCCESS)
		{
			free(tmp);
			free_data(data);
			ft_free_tab(data->cmd_split);
			return (1);
		}
		free(data->user_input);
	}
	data->raw_input = tmp;
	free_token(data);
	return (0);

}
void	minishell_prompt(t_data *data)
{
	char	**cmd_list;
	int		i;

	while(1)
	{
		signals_interact();
		prompt_user(data);
		signals_no_interact();
		data->raw_input = data->user_input;
		data->cmd_split = ft_split(data->user_input, ';');
		if (check_error_raw(data))
			continue;
		i = -1; 
		while (data->cmd_split[++i])
		{
			data->user_input = ft_strdup(data->cmd_split[i]);
			if (i == 1)
			{
				free(data->raw_input);
				data->raw_input = NULL;
			}
			scan_input(data);
			// print_token(data->token_root);
			if (check_error(data) == EXIT_SUCCESS)
			{
				parse_token(data);
				parse_near_quote(data);
				build_cmd_list(data, *data->token_root);
				if (init_io_redir(data) == EXIT_SUCCESS)
					execute(data);
			}
			free_data(data);
			dup2(data->old_fd[0], 0);
			// dup2(data->old_fd[1], 1);
		}
		ft_free_tab(data->cmd_split);
	}
}
