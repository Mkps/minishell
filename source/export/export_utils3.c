/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:25:55 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/28 11:53:38 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_wordsize(char *s, char c, int pos)
{
	int	wsize;

	wsize = 0;
	while (s[pos] != c && s[pos])
	{
		wsize++;
		pos++;
	}
	return (wsize);
}

char	*add_quotes(char *str)
{
	size_t	len;
	char	*result;

	len = ft_strlen(str);
	result = (char *)malloc(len + 3);
	if (result)
	{
		result[0] = '"';
		ft_strlcpy(result + 1, str, len + 2);
		result[len + 1] = '"';
		result[len + 2] = '\0';
	}
	free(str);
	return (result);
}

int	export_key_exists(t_export *export, char *key_to_check)
{
	while (export != NULL)
	{
		if (ft_strncmp(export->key, key_to_check, ft_strlen(key_to_check)) == 0)
		{
			if (ft_strlen(export->key) == ft_strlen(key_to_check))
			{
				return (1);
			}
		}
		export = export->next;
	}
	return (0);
}

int	key_is_valid(char *chaine)
{
	int	i;

	i = 0;
	if (chaine[i] == '\0')
		return (0);
	if (!ft_isalpha(chaine[i]) && chaine[i] != '_')
		return (1);
	while (chaine[i] != '\0')
	{
		if (!ft_isalnum(chaine[i]) && chaine[i] != '_')
			return (1);
		i++;
	}
	return (0);
}
