/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 13:05:35 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/27 13:14:36 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env_unset_free(t_env *current)
{
	free(current->key);
	free(current->value);
	free(current);
}

void	export_unset_free(t_export *current)
{
	free(current->key);
	free(current->value);
	free(current);
}

t_cmd	*find_unset_command(t_data *data)
{
	int		i;
	t_cmd	*cmd;
	t_cmd	**cmd_list;

	cmd_list = data->cmd_list;
	i = 0;
	while (cmd_list && cmd_list[i])
	{
		cmd = cmd_list[i];
		if (cmd->args && cmd->args[0] && ft_strncmp(cmd->args[0],
				"unset", ft_strlen("unset")) == 0)
			return (cmd);
		i++;
	}
	return (NULL);
}
