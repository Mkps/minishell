/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:34:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 07:03:21 by aloubier         ###   ########.fr       */
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

void	manage_expand(t_data *data, t_token **current)
{
	t_token	*tmp;

	tmp = *current;
	var_expander(data, tmp->value, &tmp);
	if (tmp->quote_status == NONE && tmp->next
		&& (!tmp->value || (tmp->value
				&& tmp->value[0] == 0)))
	{
		tmp = tmp->next;
		free(tmp->prev->value);
		lst_del_prev(&tmp);
		if (tmp->prev == NULL)
			*data->token_root = tmp;
	}
	else if (tmp->quote_status == NONE
		&& tmp->prev && (!tmp->value
			|| (tmp->value && tmp->value[0] == 0)))
	{
		tmp = tmp->prev;
		free(tmp->next->value);
		lst_del_next(&tmp);
	}
	else if (tmp->quote_status == NONE
		&& (!tmp->value || (tmp->value
				&& tmp->value[0] == 0)))
	{
		free(tmp->value);
		lst_del_token(&tmp);
		data->token_root = NULL;
	}
	*current = tmp;
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
