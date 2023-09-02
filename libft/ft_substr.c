/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 18:24:57 by aloubier          #+#    #+#             */
/*   Updated: 2022/12/07 10:11:55 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_returnptr(char *str)
{
	str = malloc(sizeof (*str) * 1);
	if (!str)
		return (NULL);
	*str = 0;
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	ret = NULL;
	if (s_len < start)
		return (ft_returnptr(ret));
	if (len >= s_len)
		len = s_len - start;
	ret = malloc((len + 1) * sizeof(*ret));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		ret[i] = s[start + i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
/*
int	main(void)
{
	char	*str;
	char	*s;

	str = ft_strdup("0123456789");
	s = ft_substr(str, 9, 10);
	printf(">> str << %s\n", str);
	printf(">> s << %s\n", s);
	free(s);

	free(str);
	return (0);
}
*/
