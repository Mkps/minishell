/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:05:16 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/13 17:06:21 by aloubier         ###   ########.fr       */
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
