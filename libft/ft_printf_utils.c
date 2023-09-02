/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:55:50 by aloubier          #+#    #+#             */
/*   Updated: 2023/03/25 13:54:47 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_char(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_str(char *c)
{
	size_t	i;

	i = 0;
	if (!c)
		return (ft_str("(null)"));
	while (c[i])
	{
		ft_char(c[i]);
		i++;
	}
	return (i);
}
