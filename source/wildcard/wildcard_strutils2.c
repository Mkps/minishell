/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_strutils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:36:20 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/25 14:29:01 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strend(char *big, char *little, char n)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(big);
	if (n > len)
		return (NULL);
	i = len - n;
	j = 0;
	while (big[i + j] && little[j])
	{
		if (big[i + j] != little[j])
			return (NULL);
		j++;
	}
	return (&big[i]);
}

char	*str_tolower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return (str);
}

int	find_length(char *str, char *src, int r_index, int n)
{
	int		str_len;
	size_t	last_index;

	if (str == NULL)
		str_len = 0;
	else
		str_len = ft_strlen(str);
	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	str_len += ft_strlen(src) + (last_index - r_index);
	return (str_len + 1);
}

// Replaces the string at r_index of length n by str
char	*str_replace_free(char *src, int r_index, int n, char *str)
{
	int		i;
	int		j;
	size_t	last_index;
	int		str_len;
	char	*ret;

	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	str_len = find_length(str, src, r_index, n);
	ret = (char *)ft_calloc(str_len + 1, sizeof(char));
	if (!ret)
		output_err("error allocating mem for return (string\n", NULL, 0);
	i = -1;
	while (++i < r_index)
		ret[i] = src[i];
	j = -1;
	while (str[++j])
		ret[i + j] = str[j];
	i = i + j;
	while (i < str_len && src[last_index])
		ret[i++] = src[last_index++];
	multi_free(src, str, NULL, NULL);
	return (ret);
}
