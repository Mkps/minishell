/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 15:11:38 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:59 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_quote_as_word(char *input, t_data *data)
{
	int	i;
	int	par_count;

	i = 0;
	par_count = 0;
	if (data->parse_status == O_PAR)
	{
		par_count++;
		while (par_count && *(input + i))
		{
			if (ft_get_sep_type(input + i) == O_PAR)
				par_count++;
			else if (ft_get_sep_type(input + i) == C_PAR)
				par_count--;
			if (par_count)
				i++;
		}
	}
	else
	{
		while (ft_get_sep_type(input + i) != TERM_END
			&& ft_get_sep_type(input + i) != data->parse_status)
			i++;
	}
	return (i);
}

void	add_empty_token(t_data *data)
{
	add_token_back(data->token_root, WORD, ft_strdup(""));
	last_token(data->token_root)->near_quote = 0;
	last_token(data->token_root)->quote_status = 1;
}

int	ft_get_word(char *input, t_data *data)
{
	int	i;
	int	current_status;

	i = 0;
	current_status = data->parse_status;
	if (data->parse_status != NONE)
		i = get_quote_as_word(input, data);
	else
		while (ft_get_sep_type(input + i) == WORD)
			i++;
	if (i == 0 && data->parse_status != NONE)
		add_empty_token(data);
	else if (i > 0)
	{
		add_token_back(data->token_root, WORD, ft_str_extract(input, i));
		last_token(data->token_root)->quote_status = current_status;
		if (!current_status && (ft_get_sep_type(input + i) == DQUOTE
				|| ft_get_sep_type(input + i) == SQUOTE))
			last_token(data->token_root)->near_quote = 1;
	}
	return (i);
}
