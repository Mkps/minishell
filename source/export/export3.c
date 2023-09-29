/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 16:57:31 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/29 13:57:32 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_envv(t_data *data)
{
	int	i;

	i = 0;
	while (data->envv[i])
	{
		free(data->envv[i]);
		i++;
	}
	free(data->envv);
}

void	free_export(t_export *node)
{
	t_export	*next;

	while (node)
	{
		next = node->next;
		free(node->value);
		free(node->key);
		free(node);
		node = next;
	}
}

void	env_assign(char ***var_split, char *variable, char **key, char **value)
{
	*var_split = ft_split2(variable, '=');
	*key = ft_strdup((*var_split)[0]);
	*value = ft_strdup((*var_split)[1]);
}

void	free_set_in(char *key, char *value, char **variable_split)
{
	if (key)
		free(key);
	if (value)
		free(value);
	if (variable_split)
		ft_free_tab(variable_split);
}

void	invalid_export_print(char *key, char *value, char **variable_split)
{
	if (key[0] == '\0')
		printf("export `': not a valid identifier\n");
	else if (value[0] != '\0')
		printf("export: `%s=%s': not a valid identifier\n", key, value);
	else
		printf("export: `%s': not a valid identifier\n", key);
	free_set_in(key, value, variable_split);
}
