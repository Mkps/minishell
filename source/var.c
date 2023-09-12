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
	char	*env;
	char	**env_p;
	char	*tmp;

	env = ft_getenv(data->envv, str);
	if (env == NULL)
	{
		free(env);
		return (NULL);
	}
	return (env);
}

// Replaces the string at r_index of length n by str
char	*str_replace(char *src, int r_index, int n, char *str)
{
	int	i;
	int		ret_len;
	int		str_len;
	int		last_index;
	char	*ret;
	
	if (str == NULL)
		str_len = 0;
	else
		str_len = ft_strlen(str);
	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	ret_len = ft_strlen(src) + (last_index - r_index) + str_len; 
	ret = (char *)malloc(sizeof (char) * (ret_len + 1));
	if (!ret)
		printf("error allocating mem for return string\n");
	i = 0;
	while (i < r_index)
	{
		ret[i] = src[i];
		i++;
	}
	while (str && *str)
		ret[i++] = *str++;
	while (i < ret_len && src[last_index])
		ret[i++] = src[last_index++];
	ret[i] = 0;
	return (ret);
}

// Replaces the $VAR with its' corresponding value stored in env if it exists.
char	*var_expander(t_data *data, char *str)
{
	int	i;
	int	n;
	
	i = 0;
	n = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				return (var_expander(data, str_replace(str, i, 2, ft_itoa(data->exit_status))));
			while (*(str + i + n) && ft_get_sep_type(str + i + n) == WORD)
				n++;
			if (n == 0)
				return (var_expander(data, str + 1));
			return (var_expander(data, str_replace(str, i, n, get_var(data, ft_str_extract(str + i + 1, n - 1)))));	
		}
		if (str[i] == 92)
		{
			if (str[i + 1] && str[i + 1] == 34)
			 return (var_expander(data, str_replace(str, i, 2, "\"")));
		}
		i++;
	}
	return (str);
}
