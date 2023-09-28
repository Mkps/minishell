/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 12:33:33 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:11 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
