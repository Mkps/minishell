/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 14:22:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 17:33:26 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*str_replace(char *src, int r_index, int n, char *str)
{
	int		i;
	int		str_len;
	size_t	last_index;
	char	*ret;

	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	str_len = find_length(str, src, r_index, n);
	ret = (char *)ft_calloc(str_len, sizeof(char));
	if (!ret)
		output_err("error allocating mem for return (string\n", NULL, 0);
	i = 0;
	while (i < r_index)
	{
		ret[i] = src[i];
		i++;
	}
	while (str && *str)
		ret[i++] = *str++;
	while (i < str_len && src[last_index])
		ret[i++] = src[last_index++];
	ret[i] = 0;
	return (ret);
}

char	*str_replace_strs(char **src, int r_index, int n, char *str)
{
	int		i;
	int		str_len;
	size_t	last_index;
	char	*ret;

	last_index = r_index + n;
	if (last_index > ft_strlen(*src))
		last_index = ft_strlen(*src);
	str_len = find_length(str, *src, r_index, n);
	ret = (char *)ft_calloc(str_len, sizeof(char));
	if (!ret)
		output_err("error allocating mem for return string\n", NULL, 0);
	i = -1;
	while (++i < r_index)
		ret[i] = *(*src + i);
	while (str && *str)
		ret[i++] = *str++;
	while (i < str_len && *(*src + last_index))
		ret[i++] = *(*src + last_index++);
	ret[i] = 0;
	free(*src);
	*src = ret;
	return (ret);
}
