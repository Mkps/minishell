/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:34:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 13:16:00 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	can_expand(t_token *current)
{
	if (current->token_type == WORD
		&& current->quote_status != SQUOTE
		&& current->quote_status != O_PAR
		&& current->quote_status != IO_HEREDOC)
	{
		return (1);
	}
	return (0);
}

int	del_empty(t_token *tmp)
{
	if (tmp && tmp->quote_status == NONE
		&& (!tmp->value || (tmp->value
				&& tmp->value[0] == 0)))
		return (1);
	return (0);
}

void	manage_expand(t_data *data, t_token **current)
{
	var_expander(data, (*current)->value, current);
	if (del_empty(*current))
	{
		if ((*current)->value)
			free((*current)->value);
		if ((*current)->next)
		{
			*current = (*current)->next;
			lst_del_prev(current);
			if ((*current)->prev == NULL)
				*data->token_root = *current;
		}
		else if ((*current)->prev)
		{
			*current = (*current)->prev;
			lst_del_next(current);
		}
		else
		{
			lst_del_token(current);
			data->token_root = NULL;
		}
	}
}

void	set_heredoc(t_token *current)
{
	{
		if (current->next && (current->next->token_type == WORD))
			current->next->quote_status = DQUOTE;
		else if (current->next
			&& ((current->next->token_type == WORD)
				|| (token_is_quote(current->next)
					&& current->next->next->token_type == WORD)))
			current->next->next->quote_status = SQUOTE;
	}
}

//	Parses tokens looking for things to expand.
void	parse_token(t_data *data)
{
	t_token	*current;

	current = *data->token_root;
	while (current != NULL)
	{
		if (can_expand(current))
			manage_expand(data, &current);
		if (current->token_type == WORD
			&& current->quote_status == NONE
			&& wc_present(current->value))
			current = wc_tokenize(current, current->value, data);
		if (current->token_type == IO_HEREDOC && current->next
			&& ((current->next->token_type == WORD)
				|| (token_is_quote(current->next)
					&& current->next->next->token_type == WORD)))
			set_heredoc(current);
		current = current->next;
	}
}
