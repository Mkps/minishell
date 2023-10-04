/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:55:50 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 12:53:06 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_char(char c)
{
	return (write(1, &c, 1));
}

int	ft_str(char *c)
{
	size_t	i;

	i = 0;
	if (!c)
		return (ft_str("(null)"));
	while (c[i])
	{
		if (ft_char(c[i]) == -1)
			return (-1);
		i++;
	}
	return (i);
}
