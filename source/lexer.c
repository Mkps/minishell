#include "../include/minishell.h"

int	ft_isspace(char c);

char	evaluate_bslash(char	*str, t_data *data)
{
	if (*(str + 1) == 0 && data->parse_status > 1)
		return ('\n');	
	else if (*(str + 1) == '\\')
		return ('\\');
	else if (*(str + 1) == '\"')
		return ('\"');
	else if (*(str + 1) == '\'')
		return ('\'');
	else
		return (0);
}

// Return 1 if a symbol corresponding to escape type is found.
int	ft_escape_seq(char *str)
{
	return (NONE);
}

// Token constructor with type and value
t_token	*create_token(int type, char *value)
{
	t_token	*ret;

	ret = malloc(sizeof(t_token));
	ret->next = NULL;
	ret->prev = NULL;
	ret->value = value;
	ret->raw_value = value;
	ret->token_type = type;
	ret->quote_status = NONE;
	return (ret);
}

t_token *last_token(t_token **root);
// Add token to the end of the provided t_token linked list.
void	add_token_back(t_token **root, int type, char *value)
{
	t_token *current;

	if (*root == NULL)
	{
		*root = create_token(type, value);
		return ;
	}
	current = *root;
	while (current->next != NULL)
		current = current->next;
	current->next = create_token(type, value);
	last_token(root)->prev = current;
}

// Duplicates the string from the starting position to n char then null-terminates it.
char	*ft_str_extract(char *str, int n)
{
	char	*ret;
	int		i;

	ret = malloc(sizeof(char) * (n + 1));
	i = 0;
	while (i < n)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = 0;
	return (ret);
}

// Returns a pointer to the last token.
t_token *last_token(t_token **root)
{
	t_token *current;

	current = *root;
	if (!current)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}
//	Returns 0 if not a separator. token_type otherwise.
int	ft_get_sep_type(char *str)
{
	if ((*str > 8 && *str < 14) || *str == 32)
		return (WSPACE);
	else if (*str == '\'')
		return (OSQUOTE);
	else if (*str == '"')
		return (ODQUOTE);
	else if (*str == '\\')
		return (BSLASH);
	else if (*str == '|' && *(str + 1) == '&')
		return (PIPE_STDERR);
	else if (*str == '|' && *(str + 1) == '|')
		return (TERM_OR);
	else if (*str == '<' && *(str + 1) == '<')
		return (IO_HEREDOC);
	else if (*str == '>' && *(str + 1) == '>')
		return (IO_APPEND);
	else if (*str == '|')
		return (PIPE);
	else if (*str == '<')
		return (IO_INPUT);
	else if (*str == '>')
		return (IO_TRUNC);
	else if (*str == 0)
		return (TERM_END);
	else if (*str == ';')
		return (TERM_SC);
	else if (*str <= 126 || *str >= 33)
		return (WORD);
	else
		return (0);
}

//Creates a new token then adds it to the end of the list.
t_token	*ft_new_token(t_token **root, char *value, int type)
{
	t_token *t;

	t = *root;
	while (t != NULL)
		t = t->next;	
	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->value = value;
	t->token_type = type;
	return (t);
}

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
