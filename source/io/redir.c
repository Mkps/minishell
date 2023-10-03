/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:15:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 12:53:57 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_fd(t_data *data, t_cmd *cmd)
{
	t_io_node	*current;

	current = *cmd->io_list;
	while (current)
	{
		if (open_fd_node(data, cmd, current))
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

int	init_io_redir(t_data *data)
{
	t_cmd	*current;

	current = *data->cmd_list;
	while (current)
	{
		if (set_hd(data, current) == EXIT_FAILURE)
			return (1);
		current = current->next;
	}
	return (0);
}

void	close_fd_node(t_cmd *cmd, t_io_node *current)
{
	if (current->fd != cmd->fd[0] && current->fd != cmd->fd[1])
		close(current->fd);
}

void	close_fd(t_data *data, t_cmd *cmd)
{
	t_io_node	*current;

	(void)data;
	current = *cmd->io_list;
	while (current)
	{
		close_fd_node(cmd, current);
		current = current->next;
	}
	if (cmd->fd[0] > -1)
		close(cmd->fd[0]);
	if (cmd->fd[1] > -1)
		close(cmd->fd[1]);
}
