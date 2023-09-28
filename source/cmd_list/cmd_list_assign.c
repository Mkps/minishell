/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_assign.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 13:10:29 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:45:51 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*set_assign(t_token *token)
{
	char	*ret;
	int		i;

	ret = ft_strdup(token->value);
	if (!ret)
		return (NULL);
	if (token->near_quote == 0)
		return (ret);
	i = 0;
	while (i <= 3)
	{
		ret = ft_strjoin(ret, token->next->value);
		token = token->next;
		token->token_type = CMD_ASSIGN;
		i++;
	}
	return (ret);
}

int	add_assign(t_token *current, t_cmd *cmd)
{
	char	*tmp;
	t_env	*env;

	tmp = set_assign(current);
	if (!tmp)
		return (EXIT_FAILURE);
	*cmd->assign = ft_lstnew_env("assign", tmp);
	free(tmp);
	env = *cmd->assign;
	if (!env)
		return (EXIT_FAILURE);
	current = current->next;
	while (current && is_assign(current->value))
	{
		tmp = set_assign(current);
		env->next = ft_lstnew_env("assign", tmp);
		env = env->next;
		free(tmp);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	handle_assign(t_data *data, t_token *token, t_cmd *cmd)
{
	t_token	*current;

	(void)data;
	current = get_cmd_first(token);
	if (current
		&& (!current->prev || (current->prev
				&& token_is_term(current->prev)))
		&& is_assign(current->value))
		return (add_assign(current, cmd));
	return (EXIT_SUCCESS);
}
