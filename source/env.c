/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:39:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 13:08:00 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/** Get the env variable from envp specified by str. */
char	*ft_getenv(char **env, const char *str)
{
	int		i;
	char	*tmp;

	tmp = 0;
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			if (*(env[i] + ft_strlen(str)) == '=')
				tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}

// Duplicates the envv string array and appends value to it.
char	**add_env_value(char **envv, char *value)
{
	int		i;
	char	**ret;
	
	i = 0;
	while (envv[i] != 0) i++;
	if (i == 0)
		return (NULL);
	ret = ft_calloc(i + 2, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (envv[i])
	{
		ret[i] = ft_strdup(envv[i]);
		if (!ret[i])
		{
			ft_free_tab(ret);
			free(ret);
			return (NULL);
		}
		i++;
	}
	ret[i] = ft_strdup(value);
	ret[i + 1] = 0;
	ft_free_tab(envv);
	return (ret);
}

// Find the value contained in string value inside envv and then replaces it.
char	**replace_env_value(char **envv, char *value)
{
	int	i;
	int	index;

	index = 0;
    while (value[index] && value[index] != '=') index++;
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
// Takes a value and checks if it exists in envv. If it does it replaces it, else it adds it.
void    ft_setenv(t_data *data, char *value)
{
    int		i;
    char	*key;
    char	*k_value;
	char	**envv;
	
	envv = data->envv;
	i = 0;
    while (value[i] && value[i] != '=') i++;
    if (i == 0 || value[i] != '=')
        return ;
    key = ft_substr(value, 0, i);
	if (!ft_getenv(envv, key))
		data->envv = add_env_value(envv, value);
	else
		data->envv = replace_env_value(envv, value);
	free(key);
}
