/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_nstd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:28:52 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 16:31:05 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_var_cmd(t_data *data, t_cmd *cmd)
{
	t_env	*start;

	start = *cmd->assign;
	if (!start)
		return ;
	while (start)
	{
		ft_setenv(data, start->value);
		start = start->next;
	}
}

void	execute_empty(t_cmd *cmd, t_data *data)
{
	int	exit_code;

	set_var_cmd(data, cmd);
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		close(data->old_fd[0]);
		close(data->old_fd[1]);
		cmd->pipe_status = 0;
		free(cmd->pipe_fd);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		exit_code = fd_valid(cmd);
		close_pipes(data->cmd_list, NULL, NULL);
		free_child(data);
		exit(exit_code);
	}
	return ;
}

void	execute_subshell(t_cmd *cmd, t_data *data)
{
	int	exit_code;
	int	ss_code;

	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		close(data->old_fd[0]);
		close(data->old_fd[1]);
		set_pipes(data, cmd);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		exit_code = fd_valid(cmd);
		close_pipes(data->cmd_list, NULL, NULL);
		ss_code = minishell_subshell(data, cmd->cmd);
		if (exit_code)
			exit(exit_code);
		exit(ss_code);
	}
}
