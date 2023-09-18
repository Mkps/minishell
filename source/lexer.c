#include "../include/minishell.h"
#include <stdlib.h>

int	ft_get_sep(char *input, t_data *data)
{
	char	*tmp;
	if (ft_get_sep_type(input) > 2)
	{
		if (ft_get_sep_type(input) == PIPE_STDERR || ft_get_sep_type(input) == IO_HEREDOC
			|| ft_get_sep_type(input) == IO_APPEND || ft_get_sep_type(input) == TERM_2AND
			|| ft_get_sep_type(input) == TERM_OR)
		{
			add_token_back(data->token_root, ft_get_sep_type(input), ft_str_extract(input, 2));
			return (2);
		}
		else
		{
			add_token_back(data->token_root, ft_get_sep_type(input), ft_str_extract(input, 1));
			return (1);
		}
	}
	return (0);
}

int	ft_get_word(char *input, t_data *data)
{
	int	i;
	int	current_status;

	i = 0;
	current_status = data->parse_status;	
	if (data->parse_status != NONE)
	{
		while (ft_get_sep_type(input + i) != TERM_END && ((ft_get_sep_type(input + i) != data->parse_status) && !(data->parse_status == O_PAR && ft_get_sep_type(input + i) == C_PAR)))
			i++;
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
		last_token(data->token_root)->quote_status= 1;
	}
	else if (i > 0)
	{
		add_token_back(data->token_root, WORD, ft_str_extract(input, i));
		last_token(data->token_root)->quote_status = current_status;
		if (!current_status && (ft_get_sep_type(input + i) == DQUOTE || ft_get_sep_type(input + i) == SQUOTE))
			last_token(data->token_root)->near_quote = 1;
	}
	return (i);
}

// Extract the quote until the end of quote.
int	ft_get_quote(char *input, t_data *data)
{
	int	i;
	int quote_type;
	t_token	*last;

	i = 1;
	add_token_back(data->token_root, ft_get_sep_type(input), ft_str_extract(input, i));
	last = last_token(data->token_root);
	if (data->parse_status == NONE && (ft_get_sep_type(input + 1) == WORD 
		|| ((ft_get_sep_type(input + 1) == DQUOTE || ft_get_sep_type(input+i) == SQUOTE) 
		&& data->parse_status == NONE)))  
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
		else if (ft_get_sep_type(input) == SQUOTE || ft_get_sep_type(input) == DQUOTE || ft_get_sep_type(input) == O_PAR)
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


// int	get_index_wc(char* str, int	start)
// {
// 	int	i;
//
// 	i = 0;
// 	if (!str)
// 		return (-1);
// 	if (ft_strlen(str) < start)
// 		return (-1);
// 	while (str[start])
// 	{
// 		if (str[start] == '*')
// 		{
// 			while (!ft_is_ws(str[start]))
// 			{
// 				start--;
// 				if (ft_is_ws(str[start]))
// 					return (start + 1);
// 			}
// 		}
// 	}
// 	return (-1);
//
// }
// void	set_index_wc(t_data *data)
// {
// 	int	i;
// 	int	pos;
//
// 	i = 0;
// 	while ((pos = get_index_wc(data->raw_input, i)) != -1)
// 	{
// 		while (data->raw_input[i] && !ft_is_ws(data->raw_input[i]))
// 			i++;
// 	}
// }
//
//Scan input for tokens then load them into the pre-command table.
int	scan_input(t_data *data)
{
	int	i;
	int	escape_type;
	char *input;
	int	input_length;

	input = data->user_input;
	data->parse_status = NONE;
	i = 0;
	if (*input == '#')
	{
		add_history(data->raw_input);
		return (EXIT_FAILURE);
	}
	while (*input && ft_is_ws(*input)) input++;
	if (input == NULL || *input == 0 || *input == '#')
		return (EXIT_FAILURE);
	add_history(data->raw_input);
	input_length = ft_strlen(input);
	while(i <= input_length)
		i += ft_get_token(input + i, data); 
	// set_index_wc(data);
	return (EXIT_SUCCESS);
} 
