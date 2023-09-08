/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 11:39:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 11:41:44 by aloubier         ###   ########.fr       */
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
	ret = ft_calloc(i, sizeof(char *));
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
