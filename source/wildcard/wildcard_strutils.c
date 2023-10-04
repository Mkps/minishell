/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_strutils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:21:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 16:03:41 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin_tab(char **tab, int i, char join)
{
	int		index;
	char	append[2];
	char	*ret;

	index = 0;
	ret = ft_strdup("");
	append[0] = join;
	append[1] = 0;
	while (index < i || (i == -1 && tab[index]))
	{
		if (tab[index])
		{
			ret = ft_strappend(ret, tab[index], 3);
			ret = ft_strappend(ret, append, 2);
		}
		index++;
	}
	free(tab);
	return (ret);
}

void	ft_str_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

char	*chrtostr(char c)
{
	char	*ret;

	ret = (char *)ft_calloc(2, sizeof(char));
	if (!ret)
		return (NULL);
	ret[0] = c;
	return (ret);
}

char	*sort_str(char *str)
{
	int		i;
	char	*ret;
	char	**split;

	split = ft_split(str, 3);
	if (split[1] == NULL)
	{
		ft_free_tab(split);
		return (str);
	}
	free(str);
	i = 0;
	while (split[i] && split[i + 1])
	{
		if (ft_strcmp_no_case(split[i], split[i + 1]) > 0)
		{
			ft_str_swap(&split[i], &split[i + 1]);
			i = 0;
		}
		else
			i++;
	}
	ret = ft_strjoin_tab(split, i + 1, ' ');
	return (ret);
}
