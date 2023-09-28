/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inline_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 16:09:52 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 11:25:22 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	inline_setup_input(t_data *data, char *user_input)
{
	char	*tmp;

	if (user_input)
	{
		tmp = ft_strdup(user_input);
		data->user_input = tmp;
		data->raw_input = tmp;
		data->cmd_split = ft_split(user_input, ';');
	}
	else
	{
		data->user_input = NULL;
		data->cmd_split = NULL;
		data->raw_input = NULL;
	}
}

void	inline_core(t_data *data)
{
	int	i;

	i = -1;
	while (data->cmd_split[++i])
	{
		data->user_input = ft_strdup(data->cmd_split[i]);
		if (i == 1)
		{
			free(data->raw_input);
			data->raw_input = NULL;
		}
		minishell_core(data);
		free_data(data);
		dup2(data->old_fd[0], STDIN_FILENO);
		dup2(data->old_fd[1], STDOUT_FILENO);
	}
}

void	minishell_inline(t_data *data, char *user_input)
{
	inline_setup_input(data, user_input);
	if (check_error_raw(data))
		exit(data->exit_status);
	inline_core(data);
	free_shell(data);
	exit(data->exit_status);
}
