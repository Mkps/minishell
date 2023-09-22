/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:05:16 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 01:07:29 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_true(void)
{
	exit (EXIT_SUCCESS);
}

int	ft_false(void)
{
	exit (EXIT_FAILURE);
}
