/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 16:03:37 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 17:25:09 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_stristr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (*little == 0)
		return (0);
	while (i < len && big[i])
	{
		j = 0;
		if (big[i] == little[j])
		{
			while (i + j < len && big[i + j] == little[j])
			{
				j++;
				if (!little[j])
					return (i);
			}
		}
		i++;
	}
	return (-1);
}

char	*tmp_wc(int *i, char *wc)
{
	int		j;
	char	*tmp;

	j = 0;
	while (wc[*i + j] && wc[*i + j] != '*')
		j++;
	tmp = ft_str_extract(wc + *i, j);
	*i += j;
	return (tmp);
}

int	is_order_matching(char *str, char *wc)
{
	int		i;
	int		index;
	char	*tmp;

	i = 0;
	index = 0;
	tmp = NULL;
	while (wc[i])
	{
		if (wc[i] == '*')
			i++;
		else
		{
			tmp = tmp_wc(&i, wc);
			if (ft_stristr(str + index, tmp, ft_strlen(str)) == -1)
				return (free(tmp), 0);
			else
				index += ft_stristr(str + index, tmp, ft_strlen(str + index));
			free(tmp);
		}
	}
	return (1);
}

char	*validate_str(char *str, char *wc)
{
	int		i;
	char	*ret;
	char	**split;

	split = ft_split(str, 3);
	free(str);
	i = 0;
	while (split[i])
	{
		if (!is_order_matching(split[i], wc))
			free_null(&split[i]);
		i++;
	}
	ret = ft_strjoin_tab(split, i, 3);
	return (ret);
}
