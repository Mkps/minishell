/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 13:22:55 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:45:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*init_new_cmd(t_data *data, t_token *token)
{
	t_cmd	*new_cmd;
	char	*tmp;
	int		i;

	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = ft_strdup(token->value);
	new_cmd->type = get_cmd_type(token);
	new_cmd->fd[0] = -2;
	new_cmd->fd[1] = -2;
	tmp = cmd_get_args(token);
	new_cmd->args = ft_split(tmp, 2);
	free(tmp);
	if (!new_cmd->args)
		return (NULL);
	i = -1;
	while (new_cmd->args[++i])
	{
		if (new_cmd->args[i][0] == 1)
			new_cmd->args[i][0] = '\0';
	}
	return (new_cmd);
}

t_cmd	*allocate_cmd(void)
{
	t_cmd	*ret;

	ret = (t_cmd *)malloc(sizeof(t_cmd) * 1);
	if (!ret)
		return (NULL);
	ret->assign = (t_env **)malloc(sizeof(t_env *) * 1);
	if (!ret)
	{
		free(ret);
		return (NULL);
	}
	ret->io_list = (t_io_node **)malloc(sizeof(t_io_node *));
	if (!ret->assign || !ret->io_list)
	{
		multi_free(ret->assign, ret, NULL, NULL);
		return (NULL);
	}
	return (ret);
}

// Creating a cmd.
t_cmd	*create_cmd(t_data *data)
{
	t_cmd	*ret;

	(void)data;
	ret = allocate_cmd();
	if (!ret)
		return (NULL);
	ret->next = NULL;
	ret->prev = NULL;
	*ret->io_list = NULL;
	*ret->assign = NULL;
	ret->pipe_status = 0;
	ret->is_bg = 0;
	ret->is_term = 0;
	return (ret);
}
