/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_noquote_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:19:22 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:16 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cycle_count_strings(int i, char *str, char separator)
{
	int	quote_status;

	quote_status = 0;
	while (str[i] && str[i] != separator)
	{
		if (ft_get_sep_type(&str[i]) == DQUOTE
			|| ft_get_sep_type(&str[i]) == SQUOTE
			|| ft_get_sep_type(&str[i]) == O_PAR)
		{
			if (ft_get_sep_type(&str[i]) == O_PAR)
				quote_status = C_PAR;
			else
				quote_status = ft_get_sep_type(&str[i]);
			i++;
			while (str[i] && ft_get_sep_type(&str[i]) != quote_status)
				i++;
		}
		else if (str[i])
			i++;
	}
	return (i);
}

int	get_extracted_strlen(char *src, char separator)
{
	int	str_len;
	int	quote_status;

	str_len = 0;
	quote_status = 0;
	while (src[str_len] && src[str_len] != separator)
	{
		if (ft_get_sep_type(&src[str_len]) == DQUOTE
			|| ft_get_sep_type(&src[str_len]) == SQUOTE
			|| ft_get_sep_type(&src[str_len]) == O_PAR)
		{
			if (ft_get_sep_type(&src[str_len]) == O_PAR)
				quote_status = C_PAR;
			else
				quote_status = ft_get_sep_type(&src[str_len]);
			str_len++;
			while (src[str_len]
				&& ft_get_sep_type(&src[str_len]) != quote_status)
				str_len++;
		}
		else if (src[str_len])
			str_len++;
	}
	return (str_len);
}

int	cycle_through(char *str, char c)
{
	int	i;
	int	quote_status;

	i = 0;
	quote_status = 0;
	while (str[i] && str[i] != c)
	{
		if (ft_get_sep_type(str + i) == DQUOTE
			|| ft_get_sep_type(str + i) == SQUOTE
			|| ft_get_sep_type(str + i) == O_PAR)
		{
			if (ft_get_sep_type(str + i) == O_PAR)
				quote_status = C_PAR;
			else
				quote_status = ft_get_sep_type(str + i);
			i++;
			while (str[i] && ft_get_sep_type(str + i) != quote_status)
				i++;
		}
		else if (str[i])
			i++;
	}
	return (i);
}
