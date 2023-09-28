/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:34:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:10 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Duplicates a string array
char	**ft_strsdup(char	**strs)
{
	int		i;
	char	**ret;

	i = 0;
	while (strs[i] != 0)
		i++;
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
			return (NULL);
		}
		i++;
	}
	return (ret);
}

/** Duplicates the values containes in envv on data->envv 
* 	allowing it to be modified in the future. **/
int	import_envv(t_data *data, char **envv)
{
	data->envv = ft_strsdup(envv);
	if (!data->envv)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
