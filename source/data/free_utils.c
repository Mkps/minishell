/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:25:25 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 10:31:01 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	free_return(int return_value, void *ptr_1, void *ptr_2, void *ptr_3)
{
	if (ptr_1 != NULL)
		free(ptr_1);
	if (ptr_2 != NULL)
		free(ptr_2);
	if (ptr_3 != NULL)
		free(ptr_3);
	return (return_value);
}

void	multi_free(void *ptr_1, void *ptr_2, void *ptr_3, void *ptr_4)
{
	if (ptr_1 != NULL)
		free(ptr_1);
	if (ptr_2 != NULL)
		free(ptr_2);
	if (ptr_3 != NULL)
		free(ptr_3);
	if (ptr_4 != NULL)
		free(ptr_4);
}

void	free_null(char **str)
{
	free(*str);
	*str = NULL;
}
