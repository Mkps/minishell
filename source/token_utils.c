/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 12:33:33 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 02:52:30 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	token_is_quote(t_token *token)
{
	if (token == NULL)
		return (0);
	if (token->token_type == DQUOTE || token->token_type == SQUOTE)
		return (1);
	return (0);
}

int	token_is_io(t_token *token)
{
	if (token == NULL)
		return (0);
	if (token->token_type == IO_INPUT || token->token_type == IO_HEREDOC
		|| token->token_type == IO_APPEND || token->token_type == IO_TRUNC)
		return (1);
	return (0);
}

int	token_is_term(t_token *token)
{
	if (token == NULL)
		return (0);
	if (token->token_type >= TERM_END && token->token_type <= TERM_OR)
		return (1);
	if (token->token_type == PIPE || token->token_type == PIPE_STDERR)
		return (1);
	return (0);
}

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
	t_token *next;
	t_token *current;

	current = *node;
	next = current->next;
	current->next = next->next;
	if (current->next)
		current->next->prev = current;
	if (next->token_type != WORD)
		free(next->value);
	free(next);
}