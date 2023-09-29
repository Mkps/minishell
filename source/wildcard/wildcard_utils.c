/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:19:05 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 15:15:58 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_start_index(char *str, int i)
{
	while (i > 0 && !ft_is_ws(str[i]))
		i--;
	if (i != 0)
		i++;
	return (i);
}

int	get_end_index(char *str, int i)
{
	while (str[i] && !ft_is_ws(str[i]))
		i++;
	return (i);
}

int	set_start_index(const char *s1, const char *s2, int *i1, int *i2)
{
	while (s1[*i1] && !ft_isalnum(s1[*i1]))
		*i1 += 1;
	while (s2[*i2] && !ft_isalnum(s2[*i2]))
		*i2 += 1;
	if (!s2[*i2] && ft_isalnum(s1[*i1]))
		return (1);
	if (!s1[*i1] && ft_isalnum(s2[*i2]))
		return (-1);
	if (*i1 > 0 && !s1[*i1])
		*i1 -= 1;
	if (*i2 > 0 && !s2[*i2])
		*i2 -= 1;
	return (0);
}

int	ft_strcmp_no_case(const char *s1, const char *s2)
{
	int	i1;
	int	i2;
	int	ret;

	i1 = 0;
	i2 = 0;
	ret = set_start_index(s1, s2, &i1, &i2);
	if (ret)
		return (ret);
	while ((ft_tolower((unsigned char)s1[i1])
			== ft_tolower((unsigned char)s2[i2])
			&& (s1[i1])))
	{
		i1++;
		i2++;
		while (s1[i1] && !ft_isalnum(s1[i1]))
			i1++;
		while (s2[i2] && !ft_isalnum(s2[i2]))
			i2++;
	}
	return (ft_tolower((unsigned char)s1[i1])
		- ft_tolower((unsigned char)s2[i2]));
}
