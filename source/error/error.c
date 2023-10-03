/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:19:22 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 10:47:03 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_par_error(t_token **root);

int	is_wc(char *str)
{
	while (*str)
		if (*str++ == '*')
			return (1);
	return (0);
}

int	check_io_error(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		if (token_is_io(tmp))
		{
			if (tmp->next && (tmp->next->token_type != WORD
					&& !token_is_quote(tmp->next)))
			{
				output_err("syntax error near unexpected token ", tmp->next, 1);
				return (SYNTAX_ERROR);
			}
			if (tmp->next && (tmp->next->token_type == WORD
					&& is_wc(tmp->next->value)))
			{
				output_err_cmd("ambiguous redirect", tmp->next->value);
				return (EXIT_FAILURE);
			}
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	check_quote_error(t_token **root)
{
	t_token	*tmp;
	int		quote_status;

	quote_status = 0;
	tmp = *root;
	while (tmp)
	{
		if (token_is_quote(tmp) && quote_status == 0)
			quote_status = tmp->token_type;
		else if (quote_status != 0 && (tmp->token_type == quote_status))
			quote_status = 0;
		tmp = tmp->next;
	}
	if (quote_status == 0)
		return (EXIT_SUCCESS);
	if (quote_status == DQUOTE)
		output_err("unexpected EOF while looking for matching `\"\'", NULL, 0);
	else if (quote_status == SQUOTE)
		output_err("unexpected EOF while looking for matching `\'\'", NULL, 0);
	return (EXIT_FAILURE);
}

int	check_term_error(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		if (token_is_term(tmp) && tmp->token_type != TERM_END
			&& (!tmp->prev || (tmp->prev && token_is_term(tmp->prev))))
		{
			output_err("syntax error near unexpected token ", tmp, 1);
			return (EXIT_FAILURE);
		}
		if (tmp->token_type == PIPE && token_is_term(tmp->next))
		{
			output_err("syntax error near unexpected token ", tmp->next, 1);
			return (EXIT_FAILURE);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	check_error(t_data *data)
{
	int	err;

	err = EXIT_SUCCESS;
	if (!err)
		err = check_io_error(data->token_root);
	if (!err && check_quote_error(data->token_root) == EXIT_FAILURE)
		err = SYNTAX_ERROR;
	if (!err && check_par_error(data->token_root) == EXIT_FAILURE)
		err = SYNTAX_ERROR;
	if (!err && check_term_error(data->token_root) == EXIT_FAILURE)
		err = SYNTAX_ERROR;
	if (err)
		data->exit_status = err;
	return (err);
}
