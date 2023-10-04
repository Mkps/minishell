/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:14:14 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 19:21:24 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_pipes(t_cmd **root, t_cmd *cmd, t_cmd *last)
{
	t_cmd	*current;

	current = *root;
	last = NULL;
	if (last)
		last = last->next;
	while (current && current->next != last)
	{
		if (current != cmd && current->pipe_status)
		{
			close(current->pipe_fd[1]);
			close(current->pipe_fd[0]);
		}
		current = current->next;
	}
}

void	set_pipes(t_data *data, t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd && cmd->prev && cmd->prev->pipe_status)
	{
		dup2(cmd->prev->pipe_fd[0], STDIN_FILENO);
	}
	if (cmd && cmd->pipe_status && cmd->fd[0])
	{
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
	}
	close_pipes(data->cmd_list, cmd, NULL);
}
