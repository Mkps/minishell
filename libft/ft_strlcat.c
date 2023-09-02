/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:55:03 by aloubier          #+#    #+#             */
/*   Updated: 2022/11/28 17:36:17 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_src;
	size_t	len_dest;
	size_t	i;

	len_src = ft_strlen(src);
	len_dest = 0;
	if (size == 0)
		return (len_src);
	while (len_dest < size && dest[len_dest])
		len_dest++;
	if (size <= len_dest)
		return (size + len_src);
	i = 0;
	while (size && (--size - len_dest) && src[i])
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[len_dest + i] = '\0';
	return (len_src + len_dest);
}
