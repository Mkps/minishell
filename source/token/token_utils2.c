/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:55:09 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 17:21:03 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strappend(char *s1, char *s2, int mode)
{
	char	*tmp;
	int		tmp_len;
	int		i;
	int		j;

	tmp_len = ft_strlen(s1) + ft_strlen(s2);
	tmp = (char *)malloc(sizeof(char) * (tmp_len + 1));
	if (!tmp)
		return (NULL);
	i = -1;
	while (s1[++i])
		tmp[i] = s1[i];
	j = -1;
	while (s2[++j])
		tmp[i + j] = s2[j];
	tmp[i + j] = 0;
	if (mode == 1)
		free(s2);
	if (mode == 2)
		free(s1);
	if (mode == 3)
		multi_free(s1, s2, 0, 0);
	return (tmp);
}

int	token_wc(char *input, t_token *current, t_data *data)
{
	int	i;

	(void)data;
	i = 0;
	if (!input)
	{
		return (1);
	}
	while (*(input + i) && ft_get_sep_type(input + i) != WSPACE)
		i++;
	if (i > 0)
		insert_token_next(current, WORD, ft_str_extract(input, i));
	while (*(input + i) != 0 && ft_get_sep_type(input + i) == WSPACE)
		i++;
	if (i == 0)
		i++;
	return (i);
}

t_token	*wc_tokenize(t_token *start, char *str, t_data *data)
{
	char	*tmp;

	(void)str;
	tmp = ft_wildcard(start->value);
	retokenize(data, tmp, start);
	return (start);
}
