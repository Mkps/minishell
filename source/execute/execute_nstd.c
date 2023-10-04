/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_nstd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:28:52 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 15:11:54 by aloubier         ###   ########.fr       */
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
	signal_parent();
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		set_fd(data, cmd);
		set_pipes(data, cmd);
		close_fd_set(data->old_fd[0], data->old_fd[1]);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		close_pipes(data->cmd_list, NULL, NULL);
		exit_code = fd_valid(cmd);
		set_sigign();
		free_child(data);
		exit(exit_code);
	}
	close_cmd_fd(cmd);
	return ;
}

void	execute_subshell(t_cmd *cmd, t_data *data)
{
	int	exit_code;
	int	ss_code;

	signal_parent();
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		set_fd(data, cmd);
		set_pipes(data, cmd);
		close_fd_set(data->old_fd[0], data->old_fd[1]);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		exit_code = fd_valid(cmd);
		close_pipes(data->cmd_list, NULL, NULL);
		set_sigign();
		ss_code = minishell_subshell(data, cmd->cmd);
		if (exit_code)
			exit(exit_code);
		exit(ss_code);
	}
	close_cmd_fd(cmd);
}
