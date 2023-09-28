/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strutils_find.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:17:52 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:19 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strs_join(char **tab)
{
	int		i;
	char	*ret;

	i = -1;
	ret = ft_strdup(" ");
	while (tab[++i])
	{
		ret = ft_strappend(ret, tab[i], 2);
		ret = ft_strappend(ret, " ", 2);
	}
	return (ret);
}

int	wsstr(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (ft_is_ws(str[i]))
			return (1);
	return (0);
}
