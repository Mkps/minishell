/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_nstd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:28:52 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:28 by aloubier         ###   ########.fr       */
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
	set_var_cmd(data, cmd);
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		close(data->old_fd[0]);
		close(data->old_fd[1]);
		cmd->pipe_status = 0;
		set_pipes(data, cmd);
		if (cmd->fd[0] > -1)
		{
			dup2(cmd->fd[0], STDIN_FILENO);
			close(cmd->fd[0]);
		}
		if (cmd->fd[1] > -1)
		{
			dup2(cmd->fd[1], STDOUT_FILENO);
			close(cmd->fd[1]);
		}
		close_pipes(data->cmd_list, NULL, NULL);
		free_child(data);
		exit(0);
	}
	return ;
}

void	execute_subshell(t_cmd *cmd, t_data *data)
{
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		close(data->old_fd[0]);
		close(data->old_fd[1]);
		set_pipes(data, cmd);
		if (cmd->fd[0] > -1)
		{
			dup2(cmd->fd[0], STDIN_FILENO);
			close(cmd->fd[0]);
		}
		if (cmd->fd[1] > -1)
		{
			dup2(cmd->fd[1], STDOUT_FILENO);
			close(cmd->fd[1]);
		}
		close_pipes(data->cmd_list, NULL, NULL);
		minishell_subshell(data, cmd->cmd);
	}
}
