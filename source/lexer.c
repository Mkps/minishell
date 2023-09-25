/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/25 14:38:23 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

int	ft_get_sep(char *input, t_data *data)
{
	if (ft_get_sep_type(input) > 2)
	{
		if (ft_get_sep_type(input) == PIPE_STDERR
			|| ft_get_sep_type(input) == IO_HEREDOC
			|| ft_get_sep_type(input) == IO_APPEND
			|| ft_get_sep_type(input) == TERM_2AND
			|| ft_get_sep_type(input) == TERM_OR)
		{
			add_token_back(data->token_root, ft_get_sep_type(input),
					ft_str_extract(input, 2));
			return (2);
		}
		else
		{
			add_token_back(data->token_root, ft_get_sep_type(input),
					ft_str_extract(input, 1));
			return (1);
		}
	}
	return (0);
}

int	ft_get_word(char *input, t_data *data)
{
	int	i;
	int	current_status;
	int	par_count;

	i = 0;
	par_count = 0;
	current_status = data->parse_status;
	if (data->parse_status != NONE)
	{
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
	}
	else
	{
		while (ft_get_sep_type(input + i) == WORD)
			i++;
	}
	if (i == 0 && data->parse_status != NONE)
	{
		add_token_back(data->token_root, WORD, ft_strdup(""));
		last_token(data->token_root)->near_quote = 0;
		last_token(data->token_root)->quote_status = 1;
	}
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

// Extract the quote until the end of quote.
int	ft_get_quote(char *input, t_data *data)
{
	int		i;
	t_token	*last;

	i = 1;
	add_token_back(data->token_root, ft_get_sep_type(input),
			ft_str_extract(input, i));
	last = last_token(data->token_root);
	if (data->parse_status == NONE && (ft_get_sep_type(input + 1) == WORD
			|| ((ft_get_sep_type(input + 1) == DQUOTE || ft_get_sep_type(input
						+ i) == SQUOTE) && data->parse_status == NONE)))
		last->near_quote = 1;
	i += (ft_get_word(input + 1, data));
	return (i);
}

//	Get a token from the user input. Extract either words or separator. Sorting between VAR and WORD will be done later.
int	ft_get_token(char *input, t_data *data)
{
	int	i;
	int	type;

	i = 0;
	type = ft_get_sep_type(input);
	if (type)
	{
		if (ft_get_sep_type(input + i) == WORD)
		{
			i = ft_get_word(input, data);
			return (i);
		}
		else if (ft_get_sep_type(input) == SQUOTE
				|| ft_get_sep_type(input) == DQUOTE)
		{
			if (data->parse_status == NONE)
				data->parse_status = ft_get_sep_type(input);
			else if (data->parse_status == ft_get_sep_type(input))
				data->parse_status = NONE;
			i += ft_get_quote(input, data);
			return (i);
		}
		else if (ft_get_sep_type(input) == O_PAR || ft_get_sep_type(input) == C_PAR)
		{
			if (data->parse_status == NONE && ft_get_sep_type(input) == O_PAR)
				data->parse_status = O_PAR;
			else if (data->parse_status == O_PAR && ft_get_sep_type(input) == C_PAR)
				data->parse_status = NONE;
			i += ft_get_quote(input, data);
			return (i);
		}
		else if (ft_get_sep_type(input + i) > 2)
		{
			i += ft_get_sep(input, data);
			return (i);
		}
		i++;
	}
	return (i);
}
int	ft_get_token_err(char *input, t_data *data)
{
	int	i;
	int	type;

	i = 0;
	type = ft_get_sep_type(input);
	if (type)
	{
		if (ft_get_sep_type(input + i) == WORD)
		{
			i = ft_get_word(input, data);
			return (i);
		}
		else if (ft_get_sep_type(input) == SQUOTE
				|| ft_get_sep_type(input) == DQUOTE)
		{
			if (data->parse_status == NONE)
				data->parse_status = ft_get_sep_type(input);
			else if (data->parse_status == ft_get_sep_type(input))
				data->parse_status = NONE;
			i += ft_get_quote(input, data);
			return (i);
		}
		else if (ft_get_sep_type(input + i) > 2)
		{
			i += ft_get_sep(input, data);
			return (i);
		}
		i++;
	}
	return (i);
}

int	ft_is_ws(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

//Scan input for tokens then load them into the pre-command table.
int	scan_input(t_data *data)
{
	int		i;
	char	*input;
	int		input_length;

	input = data->user_input;
	data->parse_status = NONE;
	i = 0;
	if (*input == '#')
	{
		add_history(data->raw_input);
		return (EXIT_FAILURE);
	}
	while (*input && ft_is_ws(*input))
		input++;
	if (input == NULL || *input == 0 || *input == '#')
		return (EXIT_FAILURE);
	if (data->raw_input)
	{
		free(data->raw_input);
		data->raw_input = NULL;
	}
	input_length = ft_strlen(input);
	while (i <= input_length)
		i += ft_get_token(input + i, data);
	return (EXIT_SUCCESS);
}
