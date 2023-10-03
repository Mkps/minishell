/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:44:45 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 13:06:34 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

	env_p = get_path(data->envv);
	cmd_p = get_cmd(cmd_node->cmd, env_p);
	if (!cmd_p || execve(cmd_p, cmd_node->args, data->envv) == -1)
	{
		if (env_p)
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
	if (!ft_strncmp(cmd, ".", 2) || !ft_strncmp(cmd, "..", 3))
		return (NULL);
	while (env_p && env_p[++i])
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
