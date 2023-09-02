/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:47:47 by aloubier          #+#    #+#             */
/*   Updated: 2022/12/07 17:22:07 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void		*ret;
	long long	total;

	total = nmemb * size;
	if (nmemb != total / size)
		return (NULL);
	ret = malloc(total);
	if (!ret)
		return (NULL);
	ft_bzero(ret, total);
	return (ret);
}
