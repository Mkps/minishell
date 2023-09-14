/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:44:45 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/13 16:40:13 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	set_fd(t_cmd *cmd)
{
	if (cmd->fd[0] != 0 || (cmd->prev && !cmd->prev->pipe_status))
		dup2(cmd->fd[0], 0);
	if (cmd->fd[1] != 1 || !cmd->pipe_status)
		dup2(cmd->fd[1], 1);
}
void	close_pipes(t_cmd **root, t_cmd *cmd)
{
	t_cmd	*current;
	current = *root;
	while (current)
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
	int res = (cmd && cmd->prev && cmd->prev->pipe_status);
	if (cmd && cmd->prev && cmd->prev->pipe_status)
	{
		dup2(cmd->prev->pipe_fd[0], STDIN_FILENO);
	}
	if (cmd && cmd->pipe_status)
	{
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
	}
	close_pipes(data->cmd_list, cmd);
}

void	exec_cmd(t_cmd *cmd_node, t_data *data)
{
	char	*cmd_p;
	char	**env_p;

	env_p = get_path(data->envv);
	cmd_p = get_cmd(cmd_node->cmd, env_p);
	if (execve(cmd_p, cmd_node->args, data->envv) == -1)
	{
		ft_free_tab(env_p);
		if (cmd_p)
			free(cmd_p);
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
