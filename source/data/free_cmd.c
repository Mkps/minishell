/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:28:54 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 15:05:46 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_cmd_cmd(t_cmd *tmp)
{
	if (tmp->type != EMPTY && tmp->args)
		ft_free_tab(tmp->args);
	if (tmp->type != EMPTY && tmp->cmd)
		free(tmp->cmd);
	if (tmp->pipe_status)
		free(tmp->pipe_fd);
	tmp->args = NULL;
	tmp->cmd = NULL;
	tmp->pipe_status = 0;
}

void	free_cmd_assign(t_cmd *cmd)
{
	t_env		*env;
	t_env		*next;

	if (cmd->assign)
	{
		env = *cmd->assign;
		while (env)
		{
			next = env->next;
			free(env->key);
			free(env);
			env = next;
		}
		free(cmd->assign);
	}
}

void	free_io_list(t_io_node *io)
{
	t_io_node	*next_io;

	while (io)
	{
		if (io->filename && io->filename[0] == 4)
			free (io->filename);
		next_io = io->next;
		if (io->fd > -1)
			close(io->fd);
		if (io->mode == IO_HEREDOC)
		{
			if (io->filename && (io->fd > -1))
			{
				unlink(io->filename);
				free(io->filename);
			}
		}
		free(io);
		io = next_io;
	}
}

void	free_cmd_io(t_data *data, t_cmd *tmp)
{
	t_io_node	*io;

	if (tmp->io_list)
	{
		io = *tmp->io_list;
		free_io_list(io);
		free(tmp->io_list);
		dup2(data->old_fd[0], STDIN_FILENO);
		dup2(data->old_fd[1], STDOUT_FILENO);
	}
}

void	free_cmd_list(t_data *data)
{
	t_cmd	*current;
	t_cmd	*tmp;

	if (*data->cmd_list == NULL)
		return ;
	current = *data->cmd_list;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		free_cmd_cmd(tmp);
		free_cmd_assign(tmp);
		free_cmd_io(data, tmp);
		free(tmp);
	}
	*data->cmd_list = NULL;
}
