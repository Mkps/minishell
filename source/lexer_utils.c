#include "../include/minishell.h"

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

//	Returns 0 if not a separator. token_type otherwise.
int	ft_get_sep_type(char *str)
{
	if ((*str > 8 && *str < 14) || *str == 32)
		return (WSPACE);
	else if (*str == '\'')
		return (OSQUOTE);
	else if (*str == '"')
		return (ODQUOTE);
	else if (*str == '(')
		return (O_PAR);
	else if (*str == ')')
		return (C_PAR);
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
