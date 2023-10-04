/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:29:45 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/04 14:18:47 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_pwd(char *pwd, char *dir)
{
	if (pwd == NULL && dir[0] != '/')
	{
		output_err_cmd(strerror(errno), "getcwd");
		return (1);
	}
	return (0);
}

int	cd_env_update(t_data *data, size_t i, size_t env_count)
{
	size_t		key_len;
	size_t		value_len;
	t_env		*current;

	current = *data->env_cpy;
	if (data->envv)
		ft_free_tab(data->envv);
	data->envv = (char **)malloc((env_count + 1) * sizeof(char *));
	if (data->envv == NULL)
		return (printf("cd_env_update: Malloc failed\n"), 1);
	while (current != NULL)
	{
		key_len = ft_strlen(current->key);
		value_len = ft_strlen(current->value);
		data->envv[i] = (char *)malloc(key_len + value_len + 2);
		if (data->envv[i] == NULL)
			return (printf("cd_env_update: Malloc failed\n"), 1);
		ft_strlcpy(data->envv[i], current->key, key_len + 1);
		data->envv[i][key_len] = '=';
		ft_strlcpy(data->envv[i] + key_len + 1, current->value, value_len + 1);
		i++;
		current = current->next;
	}
	data->envv[env_count] = NULL;
	return (0);
}
