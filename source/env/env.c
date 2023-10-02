/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:39:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/02 19:24:28 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getenv(char **env, const char *str)
{
	int		i;
	char	*tmp;

	tmp = 0;
	i = 0;
	if (!str || !str[i])
		return (NULL);
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
		{
			if (*(env[i] + ft_strlen(str)) == '='
				&& *(env[i] + ft_strlen(str) + 1) != '\0')
				tmp = env[i] + ft_strlen(str) + 1;
		}
		i++;
	}
	if (tmp && *tmp)
		return (tmp);
	return (NULL);
}

// Used to return NULL while freeing the ret tab
char	**duplicate_ret(char **ret, char **envv)
{
	int	i;

	i = 0;
	while (envv[i])
	{
		ret[i] = ft_strdup(envv[i]);
		if (!ret[i])
		{
			ft_free_tab(ret);
			output_err_cmd("Error adding value to environment.\n", NULL);
			return (NULL);
		}
		i++;
	}
	return (ret);
}

// Duplicates the envv string array and appends value to it.
char	**add_env_value(char **envv, char *value)
{
	int		i;
	char	**ret;

	i = 0;
	while (envv[i] != 0)
		i++;
	ret = (char **)ft_calloc(i + 2, sizeof(*ret));
	if (!ret)
	{
		output_err_cmd("Error adding value to environment.\n", NULL);
		return (envv);
	}
	if (envv && i == 0)
		envv = ret;
	else
		ret = duplicate_ret(ret, envv);
	if (!ret)
		return (envv);
	ret[i] = ft_strdup(value);
	ft_free_tab(envv);
	free(value);
	return (ret);
}

// Find the value contained in string value inside envv and then replaces it.
char	**replace_env_value(char **envv, char *value)
{
	int	i;
	int	index;

	index = 0;
	while (value[index] && value[index] != '=')
		index++;
	i = 0;
	while (envv[i])
	{
		if (!ft_strncmp(envv[i], value, index))
		{
			free(envv[i]);
			envv[i] = ft_strdup(value);
		}
		i++;
	}
	free(value);
	return (envv);
}

void	ft_setenv(t_data *data, char *value)
{
	int		i;
	char	*key;
	char	**envv;

	envv = data->envv;
	i = 0;
	while (value[i] && value[i] != '=')
		i++;
	if (i == 0 || value[i] != '=')
		return ;
	key = ft_substr(value, 0, i);
	if (!ft_getenv(envv, key))
		data->envv = add_env_value(envv, value);
	else
		data->envv = replace_env_value(envv, value);
	free(key);
}
