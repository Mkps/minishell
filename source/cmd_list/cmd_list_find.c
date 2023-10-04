/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_find.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:41:32 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 16:08:06 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*get_cmd_first(t_token *current_t)
{
	t_token	*current;

	current = current_t;
	if (current->prev == NULL)
		return (current);
	while (current != NULL && current->prev != NULL
		&& !token_is_term(current->prev))
		current = current->prev;
	return (current);
}

int	is_invalid_cmd(t_token *current)
{
	if ((current->prev && token_is_io(current->prev))
		|| (current->value && !current->value[0]
			&& current->quote_status == NONE))
		return (1);
	return (0);
}

// Returns the first encountered cmd token
t_token	*get_next_cmd(t_token *src)
{
	t_token	*current;

	current = src;
	while (current != NULL && !token_is_term(current))
	{
		if (current && current->token_type == WORD
			&& (!is_assign(current->value) || current->quote_status == O_PAR))
		{
			if (is_invalid_cmd(current))
			{
				current = current->next;
				continue ;
			}
			else if (current->prev == NULL || (current->quote_status == NONE
					&& current->prev != NULL && !token_is_io(current->prev)))
				return (current);
			else if (current->prev != NULL && current->quote_status != NONE
				&& !token_is_io(current->prev->prev))
				return (current);
		}
		current = current->next;
	}
	if (token_is_term(current))
		return (current);
	return (NULL);
}

// Returns the last cmd of the cmd_list
t_cmd	*last_cmd(t_cmd **root)
{
	t_cmd	*current;

	current = *root;
	if (!current)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}
