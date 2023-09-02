/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:34:06 by aloubier          #+#    #+#             */
/*   Updated: 2022/12/07 09:24:59 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*pos;

	pos = (char *)str;
	str = str + ft_strlen(str);
	while (str + 1 != pos)
	{
		if (*str == c)
			return ((char *)str);
		str--;
	}
	if ((char)c == *pos)
		return ((char *)pos);
	return (NULL);
}
