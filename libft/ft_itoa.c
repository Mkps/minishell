/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 13:06:51 by aloubier          #+#    #+#             */
/*   Updated: 2022/12/07 09:30:24 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	size_itoa(int n)
{
	int		size;
	long	nb;

	size = 0;
	nb = n;
	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		nb *= -1;
		size++;
	}
	while (nb > 0)
	{
		nb /= 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	long	nb;
	char	*s;
	size_t	size;

	size = size_itoa(n);
	nb = n;
	s = malloc(sizeof(char) * (size_itoa(n) + 1));
	if (!s)
		return (NULL);
	s[size] = '\0';
	size--;
	if (nb == 0)
		s[0] = '0';
	if (nb < 0)
	{
		s[0] = '-';
		nb *= -1;
	}
	while (nb > 0)
	{
		s[size] = nb % 10 + '0';
		nb /= 10;
		size--;
	}
	return (s);
}
/*
#include <stdio.h>

int		main(void)
{
	printf("\nResult is [%s]\n", ft_itoa(0));

}
*/
