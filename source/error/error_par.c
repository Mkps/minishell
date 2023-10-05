/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_par.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:20:11 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/05 11:54:39 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	par_error_start(t_token *tmp)
{
	if (tmp->token_type == O_PAR && tmp->prev
		&& (!token_is_term(tmp->prev) && tmp->prev->token_type != O_PAR)
		&& tmp->prev->token_type != WORD)
	{
		if (!tmp->prev->prev && tmp->prev->token_type == WORD)
			output_err("syntax error near unexpected token ",
				tmp->next, 1);
		else
			output_err("syntax error near unexpected token `('", NULL, 0);
		return (EXIT_FAILURE);
	}
	if (tmp->token_type == O_PAR && token_is_term(tmp->next))
	{
		output_err("syntax error near unexpected token ",
			tmp->next, 1);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_par_loop(t_token *tmp, int par_status)
{
	while (tmp)
	{
		if (tmp->token_type == O_PAR)
			par_status++;
		if (par_status == 0 && tmp->token_type == C_PAR)
			return (output_err_ret(-1,
					"syntax error near unexpected token `)'", NULL));
		if (par_error_start(tmp) == EXIT_FAILURE)
			return (-1);
		if (tmp->token_type == C_PAR)
		{
			if (tmp->prev->token_type == O_PAR)
				return (output_err_ret(-1,
						"syntax error near unexpected token `)'", NULL));
			par_status--;
		}
		tmp = tmp->next;
	}
	return (par_status);
}

int	check_par_error(t_token **root)
{
	t_token	*tmp;
	int		par_status;

	par_status = 0;
	tmp = *root;
	par_status = check_par_loop(tmp, par_status);
	if (par_status == -1)
		return (EXIT_FAILURE);
	if (par_status != 0)
		return (output_err_ret(EXIT_FAILURE,
				"unexpected EOF while looking for matching ')'", NULL));
	return (EXIT_SUCCESS);
}
