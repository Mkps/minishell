/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:36:35 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:56 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_ws(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

// Duplicates the string from the starting position
// to n char then null-terminates it.
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

char	*ft_str_extract_free(char *str, int n)
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
	free(str);
	return (ret);
}

int	ft_get_sep_type_expand(char *str)
{
	if (*str == '>' && *(str + 1) == '>')
		return (IO_APPEND);
	else if (*str == '|')
		return (PIPE);
	else if (*str == '&')
		return (TERM_AND);
	else if (*str == '<')
		return (IO_INPUT);
	else if (*str == '>')
		return (IO_TRUNC);
	else if (*str == 0)
		return (TERM_END);
	else if (*str == ';' && *(str + 1) == ';')
		return (TERM_2SC);
	else if (*str == ';')
		return (TERM_SC);
	else if (*str <= 126 || *str >= 33)
		return (WORD);
	else
		return (0);
}

//	Returns 0 if not a separator. token_type otherwise.
int	ft_get_sep_type(char *str)
{
	if ((*str > 8 && *str < 14) || *str == 32)
		return (WSPACE);
	else if (*str == '\'')
		return (SQUOTE);
	else if (*str == '"')
		return (DQUOTE);
	else if (*str == '(')
		return (O_PAR);
	else if (*str == ')')
		return (C_PAR);
	else if (*str == '|' && *(str + 1) == '&')
		return (PIPE_STDERR);
	else if (*str == '&' && *(str + 1) == '&')
		return (TERM_2AND);
	else if (*str == '|' && *(str + 1) == '|')
		return (TERM_OR);
	else if (*str == '<' && *(str + 1) == '<')
		return (IO_HEREDOC);
	else
		return (ft_get_sep_type_expand(str));
}
