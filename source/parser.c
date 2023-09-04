#include "../include/minishell.h"

char	*str_env(t_data *data, char *str)
{
	int		i;
	char	*tmp;
	char	**env;

	tmp = 0;
	i = 0;
	env = data->envv;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}
//Returns the value of the var. NULL if not set.
char	*get_var(t_data *data,char *str)
{
	int		i;
	char	*ret;
	char	*tmp;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != ' ')	
		i++;
	if (i == 0)
		return (NULL);
	tmp = ft_str_extract(str, i);
	ret = str_env(data, str);
	return (ret);
}

// Replaces the string at r_index of length n by str
char	*str_replace(char *src, int r_index, int n, char *str)
{
	int	i;
	int		ret_len;
	int		last_index;
	char	*ret;
	
	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	ret_len = ft_strlen(src) + (last_index - r_index) + ft_strlen (str); 
	ret = (char *)malloc(sizeof (char) * (ret_len + 1));
	i = -1;
	while (++i < r_index)
		ret[i] = src[i];
	while (*str)
		ret[i++] = *str++;
	while (i < ret_len)
		ret[i++] = src[last_index++];
	return (ret);
}

// Replaces the $VAR with its' corresponding value stored in env if it exists.
char	*var_expander(t_data *data, char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			 return (get_var(data, str + i));	
		}
	}
	return (NULL);
}
