/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_get.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:59:33 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:22 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	wc_in_dirpath(char *filepath)
{
	int	i;

	i = 0;
	while (filepath[i])
		i++;
	while (i && filepath[i] != '/')
		i--;
	if (!i)
		return (0);
	while (i && filepath[i] != '*')
		i--;
	if (!i)
		return (0);
	return (1);
}

char	*get_fwc(char *str)
{
	char	*f_wc;
	char	*ret;
	int		i;

	if (wc_in_dirpath(str))
		return (NULL);
	f_wc = get_front_wc(str);
	if (f_wc)
		i = ft_strlen(f_wc) + 1;
	ret = get_wc_data(f_wc, NULL, 0);
	while (f_wc != NULL)
	{
		free(f_wc);
		f_wc = get_front_wc(str + i);
		if (f_wc)
		{
			i += ft_strlen(f_wc) + 1;
			ret = get_wc_data(f_wc, ret, 2);
		}
	}
	return (ret);
}

char	*get_wildcard(char *str)
{
	char	*b_wc;
	char	*ret;

	ret = get_fwc(str);
	if (!ret)
		return (str);
	b_wc = get_back_wc(str);
	if (b_wc)
	{
		ret = get_wc_data(b_wc, ret, 1);
		free(b_wc);
	}
	if (ret[0] == 0)
	{
		free(ret);
		return (str);
	}
	free(str);
	ret = sort_str(ret);
	if (ret)
		ret[ft_strlen(ret) - 1] = 0;
	return (ret);
}
