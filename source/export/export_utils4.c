/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 17:24:09 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/04 13:42:18 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_lstsize_env(t_env **lst)
{
	size_t	size;
	t_env	*current;

	size = 0;
	current = *lst;
	while (current)
	{
		current = current->next;
		size++;
	}
	return (size);
}

int	print_envp(t_data *data)
{
	int	i;

	i = 0;
	while (data->envv[i])
	{
		if (ft_printf("%s\n", data->envv[i]) == -1)
			return (perror("minishell: env"), 125);
		i++;
	}
	return (EXIT_SUCCESS);
}
