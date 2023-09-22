/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:23 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 01:30:01 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
char	*get_var(t_data *data, char *str)
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

char	*str_replace(char *src, int r_index, int n, char *str)
{
	int		i;
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
	ret = (char *)malloc(sizeof(char) * (ret_len + 1));
	if (!ret)
		printf("error allocating mem for return (string\n"));
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

char	*str_replace_strs(char **src, int r_index, int n, char *str)
{
	int		i;
	int		ret_len;
	int		str_len;
	int		last_index;
	char	*ret;

	if (str == NULL)
		str_len = 0;
	else
		str_len = ft_strlen(str);
	last_index = r_index + n;
	if (last_index > ft_strlen(*src))
		last_index = ft_strlen(*src);
	ret_len = ft_strlen(*src) + (last_index - r_index) + str_len;
	ret = (char *)malloc(sizeof(char) * (ret_len + 1));
	if (!ret)
		printf("error allocating mem for return (string\n"));
	i = -1;
	while (++i < r_index)
		ret[i] = *(*src + i);
	while (str && *str)
		ret[i++] = *str++;
	while (i < ret_len && *(*src + last_index))
		ret[i++] = *(*src + last_index++);
	ret[i] = 0;
	free(*src);
	*src = ret;
	return (ret);
}
int	is_valid_var(char *str)
{
	int	i;

	if (*str == 0 || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		i++;
	}
	return (1);
}

int	var_is_multiple(char *var)
{
	int	i;
	int	ws;

	i = -1;
	if (!var)
		return (0);
	while (var[++i])
		if (var[i] == ' ')
			return (1);
	return (0);
}

int	retokenize(t_data *data, char *str, t_token *token)
{
	char	**split;
	t_token	*new_token;
	t_token	*current;
	int		i;

	current = token;
	split = ft_split(str, ' ');
	if (!split)
		return (EXIT_FAILURE);
	token->value = split[0];
	i = 1;
	while (split[i])
	{
		insert_token_next(current, WORD, ft_strdup(split[i]));
		current = current->next;
		i++;
	}
	return (EXIT_SUCCESS);
}
