/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_wcutils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:23:50 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/02 13:37:27 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stddef.h>

char	*get_front_wc(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (NULL);
	while (str[i] && str[i] != '*')
		i++;
	if (str[i])
		return (ft_str_extract(str, i));
	return (NULL);
}

char	*get_back_wc(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	while (str[i] != '*' && i > 0)
		i--;
	if (str[i] == '*' && str[i + 1] == 0)
		return (NULL);
	return (ft_str_extract(str + i + 1, ft_strlen(str) - (i + 1)));
}

int	wc_present(char *str)
{
	while (*str)
		if (*str++ == '*')
			return (1);
	return (0);
}

size_t	wc_minlen(char *str)
{
	size_t	count;
	size_t	len;
	int		i;

	i = -1;
	count = 0;
	len = 0;
	while (str[++i])
	{
		if (str[i] == '/')
		{
			len = 0;
			count = 0;
		}
		if (str[i] == '*')
			count++;
		len++;
	}
	if (len == count && count != 0)
		return (0);
	return (len - count);
}
