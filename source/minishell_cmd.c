/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:44:45 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/21 13:05:42 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static char	**escape_quote(char *cmd, char ***cmd_split, char *sep)
{
	char	**sq;

	sq = NULL;
	if (cmd_split[0][1])
	{
		if (cmd_split[0][1][0] == 34 || cmd_split[0][1][0] == 39)
		{
			*sep = cmd_split[0][1][0];
			free(cmd_split[0][1]);
			sq = ft_split(cmd, *sep);
			cmd_split[0][1] = sq[1];
			cmd_split[0][2] = NULL;
		}
	}
	return (sq);
}
int	open_fd_node(t_data *data, t_cmd *cmd, t_io_node *fd)
{
	int		tmp_fd;
	int		status;
	pid_t	pid;
	if (fd->mode == IO_INPUT)
	{
		fd->fd = open_fd(0, fd->filename);
		if (fd->fd > 0)
		{
			if (cmd->fd[0] >= 0)
				close(cmd->fd[0]);
			cmd->fd[0] = fd->fd;
			return (0);
		}		
		return (1);
	}
	if (fd->mode == IO_HEREDOC)
	{
		if (!here_doc_handler(data, fd->filename))
		{
			fd->fd=-42;
			return (0);
		}

		// if (fd->fd > 0)
		// {
		// 	if (cmd->fd[0] >= 0)
		// 		close(cmd->fd[0]);
		// 	cmd->fd[0] = fd->fd;
		// 	return (0);
		// }
		return (1);
	}
	if (fd->mode == IO_TRUNC)
	{
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
	if (fd->mode == IO_APPEND)
	{
		fd->fd = open_fd(2, fd->filename);
		if (fd->fd > 0)
		{
			if (cmd->fd[1] >= 0)
				close(cmd->fd[1]);
			cmd->fd[1] = fd->fd;
			return (0);
		}		
		return (1);
	}
	return (0);

}
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

int		init_io_redir(t_data *data)
{
	t_cmd	*current;

	current = *data->cmd_list;
	while (current)
	{
		if (set_fd(data, current) == EXIT_FAILURE)
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

	current = *cmd->io_list;
	while (current)
	{
		close_fd_node(cmd, current);
		current = current->next;
	}
	if (cmd->fd[0] > 0)
		close(cmd->fd[0]);
	if (cmd->fd[1] > 0)
		close(cmd->fd[1]);
}
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
	if (cmd && cmd->pipe_status)
	{
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
	}
	close_pipes(data->cmd_list, cmd, NULL);
}
char	*ft_strs_join(char **tab)
{
	int	i;
	char	*ret;

	i = -1;
	ret = ft_strdup(" ");
	while (tab[++i])
	{
		ret = ft_strappend(ret, tab[i], 2);
		ret = ft_strappend(ret, " ", 2);
	}
	return (ret);
}

int		wsstr(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (ft_is_ws(str[i]))
			return (1);
	return (0);
}
void	extract_cmd(t_cmd *cmd_node, t_data *data)
{
	char	*cmd_p;
	char	**env_p;
	char	**cmd_split;
	char	*arg_str;

	env_p = get_path(data->envv);
	arg_str = ft_strs_join(cmd_node->args);
	cmd_split = ft_split(arg_str, ' ');
	cmd_p = get_cmd(cmd_split[0], env_p);
	if (!cmd_p || execve(cmd_p, cmd_split, data->envv) == -1)
	{
		ft_free_tab(env_p);
		if (cmd_p)
			free(cmd_p);
	}
}
void	exec_cmd(t_cmd *cmd_node, t_data *data)
{
	char	*cmd_p;
	char	**env_p;

	if (wsstr(cmd_node->cmd))
		extract_cmd(cmd_node, data);
	else
	{
		env_p = get_path(data->envv);
		cmd_p = get_cmd(cmd_node->cmd, env_p);
		if (!cmd_p || execve(cmd_p, cmd_node->args, data->envv) == -1)
		{
			ft_free_tab(env_p);
			if (cmd_p)
				free(cmd_p);
		}
	}
}

char	*get_cmd(char *cmd, char **env_p)
{
	int		i;
	char	*cmd_dir;
	char	*cmd_tmp;

	i = -1;
	while (env_p[++i])
	{
		cmd_dir = ft_strjoin(env_p[i], "/");
		cmd_tmp = ft_strjoin(cmd_dir, cmd);
		free(cmd_dir);
		if (access(cmd_tmp, F_OK) == 0)
		{
			return (cmd_tmp);
		}
		free(cmd_tmp);
	}
	if (access(cmd, F_OK) == 0)
	{
		if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1))
			return (ft_strdup(cmd));
	}
	return (NULL);
}
