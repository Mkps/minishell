/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 12:33:42 by aloubier          #+#    #+#             */
/*   Updated: 2022/11/28 18:15:52 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*tmp;

	tmp = dest;
	if (src < dest)
	{
		src += n;
		dest += n;
		while (n--)
			*(char *)--dest = *(char *)--src;
	}
	else
	{
		while (n--)
			*(char *)dest++ = *(char *)src++;
	}
	return (tmp);
}
