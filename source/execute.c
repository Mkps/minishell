/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/27 11:18:32 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

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
	data->exit_status = execute_builtin(cmd, data);
}

int	close_fd_set(int fdin, int fdout)
{
	if (fdin > -1)
		close(fdin);
	if (fdout > -1)
		close(fdout);
	return (EXIT_SUCCESS);
}

int	close_cmd_fd(t_cmd *cmd)
{
	t_io_node	*io;
	t_io_node	*next;

	if (cmd->io_list)
	{
		io = *cmd->io_list;
		while (io)
		{
			next = io->next;
			if (io->fd > -1)
				close(io->fd);
			io = next;
		}
	}
	return (EXIT_SUCCESS);
}

int	dup_close_fd_set(int fdin, int fdout)
{
	if (fdin > -1)
	{
		dup2(fdin, STDIN_FILENO);
		close(fdin);
	}
	if (fdout > -1)
	{
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
	}
	return (EXIT_SUCCESS);
}

void	execute_child(t_cmd *cmd, t_data *data)
{
	int	exit_code;

	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		close_fd_set(data->old_fd[0], data->old_fd[1]);
		set_pipes(data, cmd);
		dup_close_fd_set(cmd->fd[0], cmd->fd[1]);
		close_pipes(data->cmd_list, NULL, NULL);
		exit_code = execute_builtin(cmd, data);
		if (exit_code == -1)
		{
			exec_cmd(cmd, data);
			exit_code = get_cmd_ecode(cmd, data);
		}
		free_child(data);
		exit(exit_code);
	}
	else
		close_cmd_fd(cmd);
}

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
	int	i;
	int	status;

	(void)data;
	i = 1;
	status = 0;
	while (i > 0 && cmd)
	{
		i -= cmd->is_term;
		close_fd_set(cmd->fd[0], cmd->fd[1]);
		close_pipes(&cmd, NULL, last);
		if (cmd->pid > 0)
			waitpid(cmd->pid, &status, 0);
		if (cmd->is_term != 0)
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
