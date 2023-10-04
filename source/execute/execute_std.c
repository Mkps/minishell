/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_std.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:29:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 18:10:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <signal.h>

int	is_standalone(t_cmd *cmd)
{
	if (is_unpiped(cmd) == 1 && cmd->is_term
		&& (!cmd->prev || (cmd->prev && cmd->prev->is_term)))
		return (1);
	return (0);
}

void	execute_parent(t_cmd *cmd, t_data *data)
{
	cmd->pid = -2;
	set_fd(data, cmd);
	dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
	data->exit_status = execute_builtin(cmd, data);
}

int	fd_valid(t_cmd *cmd)
{
	if (cmd->fd[0] == -1)
		return (EXIT_FAILURE);
	if (cmd->fd[1] == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	execute_child(t_cmd *cmd, t_data *data)
{
	int	exit_code;

	signal_parent();
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		set_pipes(data, cmd);
		set_fd(data, cmd);
		close_fd_set(data->old_fd[0], data->old_fd[1]);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		close_pipes(data->cmd_list, NULL, NULL);
		exit_code = fd_valid(cmd);
		if (!exit_code)
			exit_code = execute_builtin(cmd, data);
		set_sigign();
		if (exit_code == -1)
		{
			exec_cmd(cmd, data);
			exit_code = get_cmd_ecode(cmd, data);
		}
		free_child(data);
		exit(exit_code);
	}
	close_cmd_fd(cmd);
}
