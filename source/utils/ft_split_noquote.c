/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_noquote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:07:52 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:17 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**ft_cleartab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static int	ft_count_strings(char *str, char separator)
{
	size_t	i;
	size_t	str_count;

	i = 0;
	str_count = 0;
	while (str[i])
	{
		while (str[i] && separator == str[i])
			i++;
		if (str[i])
			str_count++;
		i = cycle_count_strings(i, str, separator);
	}
	return (str_count);
}

static char	*ft_string_extract(char *src, char separator)
{
	int		i;
	char	*dest;
	int		str_len;

	str_len = get_extracted_strlen(src, separator);
	if (str_len == 0)
		return (0);
	dest = malloc(sizeof(char) * (str_len + 1));
	if (!dest)
		return (0);
	i = 0;
	while (i < str_len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	**ft_cycle_str(char **tab, char *str, char c)
{
	int	i;

	i = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			tab[i] = ft_string_extract((char *)str, c);
			if (tab[i] == NULL)
				return (ft_cleartab(tab));
			if (ft_count_strings(str, c) != 0)
				i++;
		}
		str += cycle_through(str, c);
	}
	tab[i] = NULL;
	return (tab);
}

char	**ft_split_noquote(char *str, char c)
{
	int		string_count;
	char	**tab;

	string_count = ft_count_strings(str, c);
	tab = malloc(sizeof(str) * (string_count + 1));
	if (!tab)
		return (NULL);
	return (ft_cycle_str(tab, str, c));
}
