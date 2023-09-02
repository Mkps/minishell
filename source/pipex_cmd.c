/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:44:45 by aloubier          #+#    #+#             */
/*   Updated: 2023/08/04 09:04:09 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_pipe(t_pipex *p, char **cmd, char **envv)
{
	int	index;

	index = 1;
	first_child(p, *cmd, envv);
	while (index < p->nb_cmd - 1)
	{
		middle_child(index, p, *(cmd + index), envv);
		index++;
	}
	last_child(index, p, *(cmd + index), envv);
	parent_handler(p);
}

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

//TODO: make it return 126 in case of !X_OK 127 !F_OK
void	cmd_error(char *cmd, int error)
{
	ft_putstr_fd(PROG_NAME, 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (error == 2)
		ft_putendl_fd("Command not found", 2);
	else
		ft_putendl_fd(strerror(error), 2);
}

void	exec_cmd(char *cmd, char **envv)
{
	char	**cmd_split;
	char	*cmd_p;
	char	**sq;
	char	sep;
	char	**env_p;

	env_p = get_path(envv);
	cmd_split = ft_split(cmd, ' ');
	sep = 0;
	sq = escape_quote(cmd, &cmd_split, &sep);
	cmd_p = get_cmd(cmd_split[0], env_p);
	if (!cmd_p || execve(cmd_p, cmd_split, envv) == -1)
	{
		cmd_error(cmd_split[0], errno);
		ft_free_tab(env_p);
		if (sep)
			ft_free_tab(sq);
		ft_free_tab(cmd_split);
		if (cmd_p != NULL)
			free(cmd_p);
	}
}

char	*get_cmd(char *cmd, char **env_p)
{
	int		i;
	char	*cmd_dir;
	char	*cmd_tmp;

	i = -1;
	if (access(cmd, F_OK) == 0)
	{
		if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1))
			return (ft_strdup(cmd));
	}
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
	return (NULL);
}
