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
	ret[i] = 0;
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
