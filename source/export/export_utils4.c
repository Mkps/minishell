/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 17:24:09 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/27 17:28:31 by uaupetit         ###   ########.fr       */
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
		printf("%s\n", data->envv[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
