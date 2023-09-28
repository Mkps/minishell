/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_find.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:41:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:21 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_dirpath(char *f_wc, int *flag)
{
	int		i;
	char	*dirpath;

	dirpath = ft_strdup(f_wc);
	i = 0;
	while (dirpath[i])
		i++;
	if (i)
		i--;
	while (i && dirpath[i] != '/')
		i--;
	if (dirpath[i] == '/')
		dirpath = ft_str_extract_free(dirpath, i);
	else
	{
		*flag = 1;
		if (!ft_strncmp(dirpath, "./", 2))
			*flag = 0;
		free(dirpath);
		dirpath = ft_strdup(".");
	}
	return (dirpath);
}

int	get_file_namepath(char *dirpath)
{
	int	i;

	i = 0;
	while (dirpath[i])
		i++;
	while (i && dirpath[i] != '/')
		i--;
	if (dirpath[i] == '/')
		i++;
	return (i);
}

int	show_hidden(char *search, char *str)
{
	int	i;

	i = get_file_namepath(search);
	if (!search || !str)
		return (0);
	if (ft_strncmp(str + i, ".", 2) == 0 || ft_strncmp(str + i, "..", 3) == 0)
		return (0);
	if (search[i] != '.' && str[i] == '.')
		return (0);
	return (1);
}

void	free_null(char **str)
{
	free(*str);
	*str = NULL;
}

char	*find_matching(char *search, char *src, char *(*function_ptr)(char *,
			char *, int), int mode)
{
	int		i;
	char	*ret;
	char	**split;

	split = ft_split(src, 3);
	free(src);
	i = 0;
	while (split[i])
	{
		if (search && (function_ptr(split[i], search,
					ft_strlen(search)) != NULL))
		{
			if (mode == 0 && !show_hidden(search, split[i]))
				free_null(&split[i]);
		}
		else
			free_null(&split[i]);
		i++;
	}
	ret = ft_strjoin_tab(split, i, 3);
	return (ret);
}
