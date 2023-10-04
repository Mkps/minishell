/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 12:25:55 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 19:21:27 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	open_input_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	(void)data;
	fd->fd = open_fd(0, fd->filename);
	if (fd->fd > 0)
	{
		cmd->fd[0] = fd->fd;
		return (0);
	}
	return (1);
}

int	open_heredoc_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	(void)data;
	fd->fd = here_doc_handler(data, fd);
	if (fd->fd > 0)
	{
		cmd->fd[0] = fd->fd;
		return (0);
	}
	return (1);
}

int	open_trunc_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	(void)data;
	fd->fd = open_fd(1, fd->filename);
	if (fd->fd > 0)
	{
		if (cmd->fd[1] >= 0)
			close(cmd->fd[1]);
		cmd->fd[1] = fd->fd;
		return (0);
	}
	return (1);
}

int	open_append_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	(void)data;
	fd->fd = open_fd(2, fd->filename);
	if (fd->fd > 0)
	{
		cmd->fd[1] = fd->fd;
		return (0);
	}
	return (1);
}

int	open_fd_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	if (fd->mode == IO_INPUT)
		return (open_input_node(data, cmd, fd));
	if (fd->mode == IO_HEREDOC && fd->fd > -1)
		return (open_input_node(data, cmd, fd));
	if (fd->mode == IO_TRUNC)
		return (open_trunc_node(data, cmd, fd));
	if (fd->mode == IO_APPEND)
		return (open_append_node(data, cmd, fd));
	return (0);
}
