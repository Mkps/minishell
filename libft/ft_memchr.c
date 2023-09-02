/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:05:08 by aloubier          #+#    #+#             */
/*   Updated: 2022/11/28 18:30:57 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char		t;
	size_t				i;
	unsigned char		*tmp;

	i = 0;
	t = (unsigned char)c;
	tmp = (unsigned char *)s;
	while (i < n)
	{
		if (*tmp == t)
			return ((void *)tmp);
		tmp += sizeof (unsigned char);
		i++;
	}
	return (NULL);
}
