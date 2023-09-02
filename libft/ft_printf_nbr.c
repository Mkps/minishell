/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_nbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:55:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/03/25 11:55:25 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putunsigned(unsigned int number, int *length)
{
	if (number > 9)
		ft_putunsigned(number / 10, length);
	*length += ft_char(number % 10 + '0');
}

int	ft_unsigned(unsigned int number)
{
	int	len;

	len = 0;
	ft_putunsigned(number, &len);
	return (len);
}

static void	ft_putnbr(int number, int *length)
{
	if (number == -2147483648)
	{
		write(1, "-2147483648", 11);
		(*length) += 11;
		return ;
	}
	if (number < 0)
	{
		*length += ft_char('-');
		ft_putnbr(number * -1, length);
	}
	else
	{
		if (number > 9)
			ft_putnbr(number / 10, length);
		*length += ft_char(number % 10 + '0');
	}
}

int	ft_number(int number)
{
	int	len;

	len = 0;
	ft_putnbr(number, &len);
	return (len);
}
