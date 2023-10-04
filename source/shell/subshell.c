/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 16:10:42 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 16:28:56 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	subshell_init(t_data *new_data, t_data *data, char *user_input)
{
	init_data(new_data);
	import_envv(new_data, data->envv);
	set_shlevel(new_data);
	new_data->user_input = ft_strdup(user_input);
	new_data->raw_input = new_data->user_input;
	new_data->cmd_split = ft_split_noquote(new_data->user_input, ';');
	free_child(data);
}

void	subshell_core(t_data *data)
{
	int	i;

	i = -1;
	while (data->cmd_split[++i])
	{
		data->user_input = ft_strdup(data->cmd_split[i]);
		if (i == 0)
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

int	minishell_subshell(t_data *data, char *user_input)
{
	t_data	new_data;

	subshell_init(&new_data, data, user_input);
	if (check_error_raw(&new_data))
		return (g_exit_code);
	subshell_core(&new_data);
	free_shell(&new_data);
	return (new_data.exit_status);
}
