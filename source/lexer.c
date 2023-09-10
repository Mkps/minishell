#include "../include/minishell.h"

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
		while (ft_get_sep_type(input + i) != TERM_END && ft_get_sep_type(input + i) != data->parse_status)
			i++;
	}
	else
	{
		while (ft_get_sep_type(input + i) == WORD)
			i++;
	}
	add_token_back(data->token_root, ft_get_sep_type(input), ft_str_extract(input, i));
	last_token(data->token_root)->quote_status = current_status;
	return (i);
}

// Extract the quote until the end of quote.
int	ft_get_quote(char *input, t_data *data)
{
	int	i;
	int quote_type;

	i = 1;
	add_token_back(data->token_root, ft_get_sep_type(input), ft_str_extract(input, i));
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
		else if (ft_get_sep_type(input) == OSQUOTE || ft_get_sep_type(input) == ODQUOTE)
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

//Scan input for tokens then load them into the pre-command table.
int	scan_input(t_data *data)
{
	int	i;
	int	escape_type;
	char *input;
	int	input_length;


	input = data->user_input;
	escape_type = NONE;
	i = 0;
	if (input == NULL)
		return (EXIT_FAILURE);
	input_length = ft_strlen(input);
	while(i <= input_length)
	{
		escape_type = ft_escape_seq(input + i);
		if (escape_type == NONE)
			i += ft_get_token(input + i, data); 
	}
	return (EXIT_SUCCESS);
} 
