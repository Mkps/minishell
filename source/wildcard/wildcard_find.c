/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_find.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 12:41:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/25 13:10:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	show_hidden(char *search, char *str)
{
	if (!search || !str)
		return (0);
	if (ft_strncmp(str, ".", 2) == 0 || ft_strncmp(str, "..", 3) == 0)
		return (0);
	if (search[0] != '.' && str[0] == '.')
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

	split = ft_split(src, '/');
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
	ret = ft_strjoin_tab(split, i, '/');
	return (ret);
}
