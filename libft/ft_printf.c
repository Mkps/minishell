/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 15:55:42 by aloubier          #+#    #+#             */
/*   Updated: 2023/03/25 13:56:21 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_select_type(va_list args, const char c)
{
	if (c == 'c')
		return (ft_char(va_arg(args, int)));
	if (c == 's')
		return (ft_str(va_arg(args, char *)));
	if (c == 'p')
		return (ft_ptr(va_arg(args, unsigned long long)));
	if (c == 'd')
		return (ft_number(va_arg(args, int)));
	if (c == 'i')
		return (ft_number(va_arg(args, int)));
	if (c == 'u')
		return (ft_unsigned(va_arg(args, unsigned int)));
	if (c == 'x' || c == 'X')
		return (ft_hex(va_arg(args, unsigned int), c));
	if (c == '%')
		return (ft_char('%'));
	else
		return (0);
}

int	ft_printf(const char *src, ...)
{
	va_list	args;
	size_t	i;
	size_t	written_char;

	i = 0;
	written_char = 0;
	va_start(args, src);
	while (src[i])
	{
		if (src[i] == '%')
		{
			written_char += ft_select_type(args, src[i + 1]);
			i++;
		}
		else
			written_char += ft_char(src[i]);
		i++;
	}
	va_end(args);
	return (written_char);
}
