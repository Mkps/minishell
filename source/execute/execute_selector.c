/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_selector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:21:48 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 07:32:02 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_echo(cmd));
	else if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_cd(cmd, data));
	else if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_pwd(data));
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_env(data));
	else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_export(data));
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_unset(data));
	else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_exit(data, cmd));
	else if (ft_strncmp(cmd->cmd, ":", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_true());
	else if (ft_strncmp(cmd->cmd, "!", ft_strlen(cmd->cmd) + 1) == 0)
		return (ft_false());
	return (-1);
}

int	is_builtin(t_cmd *cmd)
{
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, ":", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "!", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	return (0);
}

int	is_unpiped(t_cmd *cmd)
{
	if (!cmd->cmd)
		return (0);
	if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, ":", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "!", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	return (0);
}
