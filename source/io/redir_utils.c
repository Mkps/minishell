/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:26:48 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 18:13:08 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		dup2(fdin, STDIN_FILENO);
	if (fdout > -1)
		dup2(fdout, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int	open_heredoc_node(t_data *data, t_cmd *cmd, t_io_node *fd);

int	open_hd_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	if (fd->mode == IO_HEREDOC)
		return (open_heredoc_node(data, cmd, fd));
	return (0);
}

int	set_hd(t_data *data, t_cmd *cmd)
{
	t_io_node	*current;

	current = *cmd->io_list;
	while (current)
	{
		if (open_hd_node(data, cmd, current))
		{
			if (cmd->fd[0] > -1)
				close(cmd->fd[0]);
			if (cmd->fd[1] > -1)
				close(cmd->fd[1]);
			cmd->fd[0] = -1;
			cmd->fd[1] = -1;
			return (1);
		}
		current = current->next;
	}
	return (0);
}
