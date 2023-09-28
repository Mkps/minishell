/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:43:06 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:10 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	lst_del_prev(t_token **node)
{
	t_token	*prev;
	t_token	*current;

	current = *node;
	prev = current->prev;
	current->prev = prev->prev;
	if (current->prev)
		current->prev->next = current;
	if (prev->token_type != WORD)
		free(prev->value);
	free(prev);
}

void	lst_del_next(t_token **node)
{
	t_token	*next;
	t_token	*current;

	current = *node;
	next = current->next;
	current->next = next->next;
	if (current->next)
		current->next->prev = current;
	if (next->token_type != WORD)
		free(next->value);
	free(next);
}

void	lst_del_token(t_token **node)
{
	t_token	*current;

	current = *node;
	if (current->token_type != WORD)
		free(current->value);
	free(current);
}
