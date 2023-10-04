/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:59:12 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/04 13:39:25 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_to_list_xpand(t_data *data, char *key, char *value)
{
	t_env	*new_env;

	new_env = ft_lstnew_two(key, value);
	if (new_env)
	{
		ft_lstadd_back_two(data->env_cpy, new_env);
		return (EXIT_SUCCESS);
	}
	output_err_cmd("ERROR failed to initiallize new_env \n", "pwd");
	free_env_lst(data->env_cpy);
	*data->env_cpy = NULL;
	return (free_return(EXIT_FAILURE, key, value, 0));
}

void	copy_env_to_list(t_data *data)
{
	int		i;
	char	*equal_sign;
	char	*key;
	char	*value;

	i = -1;
	while (data->envv[++i])
	{
		equal_sign = ft_strrchr(data->envv[i], '=');
		if (equal_sign != NULL)
		{
			key = ft_substr(data->envv[i],
					0, equal_sign - data->envv[i]);
			value = ft_strdup(equal_sign + 1);
			if (key && value)
				if (env_to_list_xpand(data, key, value))
					return ;
		}
	}
}

void	print_env_list(t_env **env_lst)
{
	t_env	*current;

	current = *env_lst;
	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
