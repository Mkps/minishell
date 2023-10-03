/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:34:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 12:54:51 by aloubier         ###   ########.fr       */
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
	char	*tmp;
	char	**tmp_envv;

	if (!envv[0])
	{
		tmp_envv = (char **)ft_calloc(2, sizeof(char *));
		tmp = "";
		tmp_envv[0] = ft_strdup(tmp);
		tmp_envv[1] = NULL;
		data->envv = tmp_envv;
		return (EXIT_FAILURE);
	}
	data->envv = ft_strsdup(envv);
	if (!data->envv)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
