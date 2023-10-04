/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 18:15:07 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	if (cmd->type == EMPTY)
		return (execute_empty(cmd, data));
	free_var(data, cmd);
	if (cmd->type == O_PAR)
		execute_subshell(cmd, data);
	else
	{
		if (is_standalone(cmd))
			execute_parent(cmd, data);
		else
			execute_child(cmd, data);
	}
}

t_cmd	*start_exec(t_data *data, t_cmd *cmd)
{
	int		i;
	t_cmd	*start;
	t_cmd	*last;

	start = cmd;
	i = 1;
	if (!start->is_term)
	{
		cmd = cmd->next;
		while (i > 0 && cmd)
		{
			i -= cmd->is_term;
			execute_cmd(cmd, data);
			cmd = cmd->next;
		}
	}
	execute_cmd(start, data);
	if (cmd == NULL)
		last = last_cmd(data->cmd_list);
	else
		last = cmd;
	return (last);
}

void	set_exit_code(t_data *data, int status, t_cmd *cmd)
{
	if (g_exit_code == 130 && !WIFEXITED(status))
		write(1, "\n", 1);
	else if (g_exit_code == 131 && !WIFEXITED(status))
		printf("Quit (core dumped)\n");
	if (cmd->is_term == 0)
		g_exit_code = 0;
	if (cmd->is_term != 0 && !is_standalone(cmd))
	{
		if (g_exit_code > 127)
			data->exit_status = g_exit_code;
		else if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			data->exit_status = 128 + WTERMSIG(status);
		}
	}
}

t_cmd	*end_exec(t_data *data, t_cmd *cmd, t_cmd *last)
{
	int		i;
	int		status;
	t_cmd	*start;

	i = 1;
	status = 0;
	start = cmd;
	while (i > 0 && cmd)
	{
		i -= cmd->is_term;
		close_fd_set(cmd->fd[0], cmd->fd[1]);
		close_pipes(&start, NULL, last);
		if (cmd->pid > 0)
			waitpid(cmd->pid, &status, 0);
		set_exit_code(data, status, cmd);
		cmd = cmd->next;
	}
	return (cmd);
}

void	execute(t_data *data)
{
	t_cmd	*cmd;
	t_cmd	*start;
	t_cmd	*last;

	g_exit_code = 0;
	start = *data->cmd_list;
	while (start)
	{
		cmd = start;
		last = start_exec(data, cmd);
		cmd = end_exec(data, cmd, last);
		if (g_exit_code > 127)
			break ;
		start = conditional(data, cmd);
	}
}
