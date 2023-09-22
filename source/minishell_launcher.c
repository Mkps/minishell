/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_launcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:21:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/22 11:42:26 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

int		check_error_raw(t_data *data);
void	minishell_inline(t_data *data, char *user_input)
{
	char	**cmd_list;
	char	*tmp;
	int		i;

	if (user_input)
	{
		tmp = ft_strdup(user_input);
		data->user_input = tmp;
		data->raw_input =tmp;
		data->cmd_split = ft_split(user_input, ';');
	}
	else
	{
		data->user_input = NULL;
		data->cmd_split = NULL;	
		data->raw_input = NULL;
	}
	if (check_error_raw(data))
		exit(g_exit_code);
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
		 //print_token(data->token_root);
		if (check_error(data) == EXIT_SUCCESS)
		{
			parse_token(data);
			parse_near_quote(data);
			build_cmd_list(data, *data->token_root);
			if (init_io_redir(data) == EXIT_SUCCESS)
				execute(data);
			else
				close_pipes(data->cmd_list, NULL, NULL);
		}
		free_data(data);
		dup2(data->old_fd[0], STDIN_FILENO);
		dup2(data->old_fd[1], STDOUT_FILENO);
	}
	ft_free_tab(data->cmd_split);
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
			 //print_token(data->token_root);
			if (check_error(data) == EXIT_SUCCESS)
			{
				parse_token(data);
				parse_near_quote(data);
				build_cmd_list(data, *data->token_root);
				if (init_io_redir(data) == EXIT_SUCCESS)
					execute(data);
				else
					close_pipes(data->cmd_list, NULL, NULL);
			}
			free_data(data);
			dup2(data->old_fd[0], STDIN_FILENO);
			dup2(data->old_fd[1], STDOUT_FILENO);
		}
		ft_free_tab(data->cmd_split);
		data->cmd_split = NULL;
	}
}
 
