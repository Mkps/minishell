/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 07:09:01 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

t_cmd	*conditional(t_data *data, t_cmd *current)
{
	(void)data;
	while ((current && g_exit_code == 0
			&& current->prev->is_term == TERM_OR)
		|| (current && g_exit_code > 0
			&& current->prev->is_term == TERM_2AND))
	{
		if (current->is_term)
			current = current->next;
		else
		{
			while (!current->is_term)
				current = current->next;
			current = current->next;
		}
	}
	return (current);
}

t_cmd	*start_exec(t_data *data, t_cmd *cmd)
{
	int		i;
	t_cmd	*last;

	i = 1;
	while (i > 0 && cmd)
	{
		i -= cmd->is_term;
		execute_cmd(cmd, data);
		cmd = cmd->next;
	}
	if (cmd == NULL)
		last = last_cmd(data->cmd_list);
	else
		last = cmd;
	return (last);
}

t_cmd	*end_exec(t_data *data, t_cmd *cmd, t_cmd *last)
{
	int		i;
	int		status;
	t_cmd	*start;

	(void)data;
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
		if (cmd->is_term != 0 && !is_standalone(cmd))
		{
			if (WIFEXITED(status) && g_exit_code < 128)
				g_exit_code = WEXITSTATUS(status);
		}
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
