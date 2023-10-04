/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 17:07:44 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/03 15:14:38 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env_update(t_data *data)
{
	size_t	key_len;
	t_env	*current_env;

	key_len = 0;
	current_env = NULL;
	if (data->envv)
		free_envv(data);
	key_len = ft_lstsize_env(data->env_cpy);
	data->envv = (char **)malloc((key_len + 1) * sizeof(char *));
	if (!data->envv)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	key_len = 0;
	current_env = *data->env_cpy;
	env_update_utils(current_env, key_len, data);
}

void	env_update_utils(t_env *current_env, size_t key_len, t_data *data)
{
	int		i;
	size_t	value_len;
	size_t	entry_len;

	value_len = 0;
	entry_len = 0;
	i = 0;
	while (current_env)
	{
		key_len = ft_strlen(current_env->key);
		value_len = ft_strlen(current_env->value);
		entry_len = key_len + value_len + 2;
		data->envv[i] = (char *)malloc(entry_len);
		if (!data->envv[i])
		{
			perror("Minishell: Malloc failed");
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(data->envv[i], current_env->key, entry_len);
		ft_strlcat(data->envv[i], "=", entry_len);
		ft_strlcat(data->envv[i], current_env->value, entry_len);
		current_env = current_env->next;
		i++;
	}
	data->envv[i] = NULL;
}

void	envcpy_update_utils(t_data *data, t_env *new_node)
{
	t_env	*current;

	current = *data->env_cpy;
	while (current)
	{
		if (!ft_strncmp(current->key, new_node->key,
				ft_strlen(current->key) + 1))
		{
			free(current->value);
			current->value = ft_strdup(new_node->value);
		}
		current = current->next;
	}
}

void	end_split(char **variable_split)
{
	int	i;

	i = -1;
	while (variable_split[++i])
	variable_split[i] = NULL;
}
