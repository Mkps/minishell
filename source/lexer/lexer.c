/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:00 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_double_size(int type)
{
	if (type == PIPE_STDERR || type == IO_HEREDOC
		|| type == IO_APPEND || type == TERM_2AND
		|| type == TERM_OR)
		return (1);
	return (0);
}

int	ft_get_sep(char *input, t_data *data)
{
	if (ft_get_sep_type(input) > 2)
	{
		if (is_double_size(ft_get_sep_type(input)))
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
