/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:55:03 by aloubier          #+#    #+#             */
/*   Updated: 2023/03/25 13:55:41 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_hexlen(unsigned int nb)
{
	int	len;

	len = 0;
	while (nb != 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

static void	ft_puthex(unsigned int nb, const char format)
{
	if (nb >= 16)
	{
		ft_puthex(nb / 16, format);
		ft_puthex(nb % 16, format);
	}
	else
	{
		if (nb <= 9)
			ft_char(nb + '0');
		else
		{
			if (format == 'x')
				ft_char((nb - 10 + 'a'));
			else if (format == 'X')
				ft_char((nb - 10 + 'A'));
		}
	}
}

int	ft_hex(unsigned int nb, const char format)
{
	if (nb == 0)
		return (ft_char('0'));
	else
		ft_puthex(nb, format);
	return (ft_hexlen(nb));
}
