/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_near_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:23:20 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:07 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_near_quote_word(t_token *current)
{
	lst_del_next(&current);
	if (current->next && current->next->token_type == WORD)
	{
		current->value = ft_strappend(current->value, current->next->value, 3);
		lst_del_next(&current);
		if (!token_is_quote(current->next) || (token_is_quote(current->next)
				&& (current->next->quote_status == NONE
					&& current->next->near_quote == 0)))
		{
			current->near_quote = 0;
			if (current->prev)
				if (current->next && token_is_quote(current->next)
					&& current->next->near_quote == 1)
					current->near_quote = 1;
		}
	}
	else if (current->next && token_is_quote(current->next)
		&& current->next->near_quote == 1)
	{
		lst_del_next(&current);
		current->near_quote = 1;
	}
}

void	parse_near_quote_quote(t_data *data, t_token *current)
{
	lst_del_prev(&current);
	if (current->prev == NULL)
		*data->token_root = current;
	if (current->next && token_is_quote(current->next)
		&& current->next->near_quote == 0)
		lst_del_next(&current);
	else if (current->next && token_is_quote(current->next)
		&& current->next->near_quote == 1)
	{
		lst_del_next(&current);
		if (current->next->token_type == WORD)
		{
			current->value = ft_strappend(current->value,
					current->next->value, 3);
			current->near_quote = current->next->near_quote;
			lst_del_next(&current);
		}
		else if (token_is_quote(current->next))
			current->near_quote = 1;
	}
}

void	current_reset_quote(t_data *data)
{
	t_token	*current;

	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD
			&& current->near_quote == 1
			&& current->quote_status == NONE)
			if (current->next && token_is_quote(current->next))
				current->next->near_quote = 1;
		current = current->next;
	}
}

void	parse_near_quote(t_data *data)
{
	t_token	*current;

	current_reset_quote(data);
	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD)
		{
			if (current->next && current->near_quote == 1)
				parse_near_quote_word(current);
			else if (current->prev && token_is_quote(current->prev))
				parse_near_quote_quote(data, current);
			else
				current = current->next;
		}
		else if (current)
			current = current->next;
	}
}
