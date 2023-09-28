/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 15:09:03 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_parse_status_par(t_data *data, char *input)
{
	if (data->parse_status == NONE && ft_get_sep_type(input) == O_PAR)
		data->parse_status = O_PAR;
	else if (data->parse_status == O_PAR
		&& ft_get_sep_type(input) == C_PAR)
		data->parse_status = NONE;
}

void	set_parse_status_quote(t_data *data, char *input)
{
	if (data->parse_status == NONE)
		data->parse_status = ft_get_sep_type(input);
	else if (data->parse_status == ft_get_sep_type(input))
		data->parse_status = NONE;
}

//	Get a token from the user input. Extract either words or separator.
//	Sorting between VAR and WORD will be done later.
int	ft_get_token(char *input, t_data *data)
{
	int	i;

	i = 0;
	if (ft_get_sep_type(input))
	{
		if (ft_get_sep_type(input + i) == WORD)
			return (ft_get_word(input, data));
		else if (ft_get_sep_type(input) == SQUOTE
			|| ft_get_sep_type(input) == DQUOTE)
		{
			set_parse_status_quote(data, input);
			return (i + ft_get_quote(input, data));
		}
		else if (ft_get_sep_type(input) == O_PAR
			|| ft_get_sep_type(input) == C_PAR)
		{
			set_parse_status_par(data, input);
			return (i + ft_get_quote(input, data));
		}
		else if (ft_get_sep_type(input + i) > 2)
			return (i + ft_get_sep(input, data));
		i++;
	}
	return (i);
}

int	ft_get_token_err(char *input, t_data *data)
{
	int	i;

	i = 0;
	if (ft_get_sep_type(input))
	{
		if (ft_get_sep_type(input + i) == WORD)
			return (ft_get_word(input, data));
		else if (ft_get_sep_type(input) == SQUOTE
			|| ft_get_sep_type(input) == DQUOTE)
		{
			set_parse_status_quote(data, input);
			return (i + ft_get_quote(input, data));
		}
		else if (ft_get_sep_type(input + i) > 2)
			return (i + ft_get_sep(input, data));
		i++;
	}
	return (i);
}
