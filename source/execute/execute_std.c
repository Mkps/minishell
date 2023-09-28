/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_std.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:29:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 07:09:08 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_standalone(t_cmd *cmd)
{
	if (is_unpiped(cmd) == 1 && cmd->is_term
		&& (!cmd->prev || (cmd->prev && cmd->prev->is_term)))
		return (1);
	return (0);
}

void	execute_parent(t_cmd *cmd, t_data *data)
{
	set_fd(data, cmd);
	cmd->pid = -2;
	g_exit_code = execute_builtin(cmd, data);
}

void	execute_child(t_cmd *cmd, t_data *data)
{
	int	exit_code;

	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		signal(SIGPIPE, SIG_IGN);
		set_pipes(data, cmd);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		close_pipes(data->cmd_list, NULL, NULL);
		exit_code = execute_builtin(cmd, data);
		if (exit_code == -1)
		{
			exec_cmd(cmd, data);
			exit_code = get_cmd_ecode(cmd, data);
		}
		close_fd_set(data->old_fd[0], data->old_fd[1]);
		free_child(data);
		exit(exit_code);
	}
	else
		close_cmd_fd(cmd);
}
