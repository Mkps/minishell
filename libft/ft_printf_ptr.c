/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:55:36 by aloubier          #+#    #+#             */
/*   Updated: 2023/03/25 11:55:40 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_ptr_len(uintptr_t nb)
{
	size_t	len;

	len = 0;
	while (nb != 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

void	ft_put_ptr(uintptr_t nb)
{
	if (nb >= 16)
	{
		ft_put_ptr(nb / 16);
		ft_put_ptr(nb % 16);
	}
	else
	{
		if (nb <= 9)
			ft_char(nb + '0');
		else
			ft_char(nb - 10 + 'a');
	}
}

int	ft_ptr(unsigned long long ptr)
{
	size_t	len;

	len = 0;
	if (ptr == 0)
		len += ft_str("(nil)");
	else
	{
		len += write(1, "0x", 2);
		ft_put_ptr(ptr);
		len += ft_ptr_len(ptr);
	}
	return (len);
}
