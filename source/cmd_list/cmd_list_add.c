/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_add.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:41:26 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 16:04:55 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//	Adds a cmd to the back of the cmd list.
void	add_cmd_back(t_data *data)
{
	t_cmd	*last;

	if (*data->cmd_list == NULL)
	{
		*data->cmd_list = create_cmd(data);
		return ;
	}
	last = *data->cmd_list;
	while (last->next != NULL)
		last = last->next;
	last->next = create_cmd(data);
	last_cmd(data->cmd_list)->prev = last;
}

// Adds an empty command for cases like only a redir or an assign are present
void	add_empty_cmd(t_data *data)
{
	t_cmd	*new_cmd;

	add_cmd_back(data);
	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = NULL;
	new_cmd->type = EMPTY;
	new_cmd->fd[0] = -2;
	new_cmd->fd[1] = -2;
}

char	*cmd_get_args(t_token *token)
{
	char	*tmp;
	t_token	*current;

	tmp = "";
	tmp = ft_strappend(tmp, chrtostr(2), 1);
	current = token;
	while (current->token_type == WORD)
	{
		tmp = ft_strappend(tmp, chrtostr(2), 3);
		if (current->value[0] == 0 && current->quote_status != NONE)
			tmp = ft_strappend(tmp, chrtostr(1), 3);
		else
			tmp = ft_strappend(tmp, current->value, 2);
		if (current->next->token_type == SQUOTE
			|| current->next->token_type == DQUOTE)
			current = current->next->next;
		else
			current = current->next;
	}
	return (tmp);
}

t_token	*cmd_get_token(t_token *token)
{
	t_token	*current;

	current = token;
	while (current->token_type == WORD)
	{
		if (current->next->token_type == SQUOTE
			|| current->next->token_type == DQUOTE)
			current = current->next->next;
		else
			current = current->next;
	}
	return (current);
}

// Main function to add_cmd to the cmd_list.
// Currently works straight out ouf the data structure.
t_token	*add_cmd(t_data *data, t_token *token)
{
	t_cmd	*new_cmd;

	add_cmd_back(data);
	new_cmd = init_new_cmd(data, token);
	if (!new_cmd)
		return (NULL);
	return (cmd_get_token(token));
}
