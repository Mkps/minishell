/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:39:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 12:19:40 by aloubier         ###   ########.fr       */
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
		// printf("listing env i %i = %s\n", i, env[i]);
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			if (*(env[i] + ft_strlen(str)) == '=')
				tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}

// Duplicates a string array
char	**ft_strsdup(char	**strs)
{
	int		i;
	char	**ret;
	
	i = 0;
	while (strs[i] != 0) i++;
	if (i == 0)
		return (NULL);
	ret = ft_calloc(i + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		ret[i] = ft_strdup(strs[i]);
		if (!ret[i])
		{
			ft_free_tab(ret);
			free(ret);
			return (NULL);
		}
		i++;
	}
	ret[i] = 0;
	return (ret);
}

// Duplicates the values containes in envv on data->envv allowing it to be modified in the future.
int	import_envv(t_data *data, char **envv)
{
	data->envv = ft_strsdup(envv);
	if (!data->envv)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

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
	ret[i] = value;
	ret[i + 1] = value;
	ft_free_tab(envv);
	return (ret);
}

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
	if (value[i + 1] == 0)
		k_value = NULL;
	else
		k_value = ft_substr(value, i + i, ft_strlen(value));
	if (!ft_getenv(envv, key))
		data->envv = add_env_value(envv, value);
	else
		data->envv = replace_env_value(envv, value);
}