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

int		is_valid_var(char *str)
{
	int	i;

	if (*str == 0 || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 0;
	while(str[i])
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		i++;
	}
	return (1);
}

// Replaces the $VAR with its' corresponding value stored in env if it exists.
char	*var_expander(t_data *data, char *str)
{
	int		i;
	int		n;
	char	*ret;
	char	*tmp;
	char	*tmp_str;
	char	*exit_code;
	
	i = 0;
	n = 1;
	ret = ft_strdup(str);
	exit_code = ft_itoa(data->exit_status);
	while (ret[i])
	{
		if (ret[i] == '$')
		{
			if (ret[i + 1] == '?')
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, exit_code);
				free(tmp);
				i = 0;
			}
			else if (ft_isalnum(ret[i + 1]) || ret[i + 1] == '_')
			{
				n = 1;
				while (*(ret + i + n) && (ft_isalnum(ret[i + n]) || ret[i + n] == '_'))
					n++;
				if (n != 1)
				{
					tmp = ret;
					tmp_str = ft_str_extract(ret + i + 1, n - 1);
					ret = str_replace(ret, i, n, get_var(data, tmp_str));
					free(tmp_str);
					free(tmp);
				}
				i = 0;
			}
			else
				i++;
		}
		else if (ret[i] == 92)
		{
			if (ret[i + 1] && ret[i + 1] == 92)
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, "\\");
				free(tmp);
			}
			i++;
		}
		else
			i++;
	}
	free(exit_code);
	free(str);
	return (ret);
}
