/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:26:48 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 19:21:50 by aloubier         ###   ########.fr       */
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
