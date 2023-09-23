/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:19:35 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 01:29:59 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_start_index(char *str, int i)
{
	while (i > 0 && !ft_is_ws(str[i]))
		i--;
	if (i != 0)
		i++;
	return (i);
}

int	get_end_index(char *str, int i)
{
	while (str[i] && !ft_is_ws(str[i]))
		i++;
	return (i);
}

char	*str_tolower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return (str);
}

int	ft_strcmp_no_case(const char *s1, const char *s2)
{
	int	i1;
	int	i2;
	int	ret;

	i1 = 0;
	i2 = 0;
	while (s1[i1] && !ft_isalnum(s1[i1]))
		i1++;
	while (s2[i2] && !ft_isalnum(s2[i2]))
		i2++;
	if (!s2[i2] && ft_isalnum(s1[i1]))
		return (1);
	if (!s1[i1] && ft_isalnum(s2[i2]))
		return (-1);
	if (i1 > 0 && !s1[i1])
		i1--;
	if (i2 > 0 && !s2[i2])
		i2--;
	while ((ft_tolower((unsigned char)s1[i1]) == ft_tolower((unsigned char)s2[i2])
			&& (s1[i1])))
	{
		i1++;
		i2++;
		while (s1[i1] && !ft_isalnum(s1[i1]))
			i1++;
		while (s2[i2] && !ft_isalnum(s2[i2]))
			i2++;
	}
	return (ft_tolower((unsigned char)s1[i1])
		- ft_tolower((unsigned char)s2[i2]));
}

char	*ft_strjoin_tab(char **tab, int i)
{
	int		index;
	int		k;
	char	*ret;

	index = 0;
	ret = ft_strdup("");
	while (index < i || (i == -1 && tab[index]))
	{
		if (tab[index])
		{
			ret = ft_strappend(ret, tab[index], 3);
			ret = ft_strappend(ret, " ", 2);
		}
		index++;
	}
	free(tab);
	return (ret);
}

void	ft_str_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

char	*sort_str(char *str)
{
	int		i;
	char	*ret;
	char	**split;

	split = ft_split(str, ' ');
	if (split[1] == NULL)
	{
		ft_free_tab(split);
		return (str);
	}
	free(str);
	i = 0;
	while (split[i] && split[i + 1])
	{
		if (ft_strcmp_no_case(split[i], split[i + 1]) > 0)
		{
			ft_str_swap(&split[i], &split[i + 1]);
			i = 0;
		}
		else
			i++;
	}
	ret = ft_strjoin_tab(split, i + 1);
	return (ret);
}

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

char	*find_matching(char *search, char *src, char *(*function_ptr)(char *,
			char *, int), int mode)
{
	int		i;
	char	*ret;
	char	**split;

	split = ft_split(src, ' ');
	free(src);
	i = 0;
	while (split[i])
	{
		if (search && (function_ptr(split[i], search,
					ft_strlen(search)) != NULL))
		{
			if (mode == 0 && !show_hidden(search, split[i]))
			{
				free(split[i]);
				split[i] = 0;
			}
		}
		else
		{
			free(split[i]);
			split[i] = 0;
		}
		i++;
	}
	ret = ft_strjoin_tab(split, i);
	return (ret);
}

char	*ft_strend(char *big, char *little, char n)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(big);
	if (n > len)
		return (NULL);
	i = len - n;
	j = 0;
	while (big[i + j] && little[j])
	{
		if (big[i + j] != little[j])
			return (NULL);
		j++;
	}
	return (&big[i]);
}

char	*get_wc(char *search, char *src, int mode)
{
	char			*str;
	char			*ret;
	DIR				*d;
	void			*select;
	struct dirent	*dir;

	if (mode == 0)
		select = &ft_strnstr;
	else if (mode == 1)
		select = &ft_strend;
	else if (mode == 2)
		select = &strstr;
	if (!src)
	{
		str = ft_strdup("");
		d = opendir(".");
		if (d)
		{
			dir = readdir(d);
			while (dir != NULL)
			{
				str = ft_strappend(str, dir->d_name, 2);
				str = ft_strappend(str, " ", 2);
				dir = readdir(d);
			}
			closedir(d);
		}
		ret = find_matching(search, str, select, mode);
	}
	else
	{
		ret = find_matching(search, src, select, mode);
	}
	return (ret);
}

char	*get_front_wc(char *str)
{
	int	i;

	i = 0;
	if (*str == 0)
		return (NULL);
	while (str[i] && str[i] != '*')
		i++;
	if (str[i])
		return (ft_str_extract(str, i));
	return (NULL);
}

char	*get_back_wc(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	while (str[i] != '*' && i > 0)
		i--;
	if (str[i] == '*' && str[i + 1] == 0)
		return (NULL);
	return (ft_str_extract(str + i + 1, ft_strlen(str) - (i + 1)));
}

int	wc_present(char *str)
{
	while (*str)
		if (*str++ == '*')
			return (1);
	return (0);
}

char	*get_wildcard(char *str)
{
	int		i;
	char	*f_wc;
	char	*b_wc;
	char	*ret;

	f_wc = get_front_wc(str);
	b_wc = get_back_wc(str);
	if (f_wc)
		i = ft_strlen(f_wc) + 1;
	ret = get_wc(f_wc, NULL, 0);
	while (f_wc != NULL)
	{
		free(f_wc);
		f_wc = get_front_wc(str + i);
		if (f_wc)
		{
			i += ft_strlen(f_wc) + 1;
			ret = get_wc(f_wc, ret, 2);
		}
	}
	if (b_wc)
	{
		ret = get_wc(b_wc, ret, 1);
		free(b_wc);
	}
	if (ret[0] == 0)
	{
		free(ret);
		return (str);
	}
	free(str);
	ret = sort_str(ret);
	if (ret)
		ret[ft_strlen(ret) - 1] = 0;
	return (ret);
}

// Replaces the string at r_index of length n by str
char	*str_replace_free(char *src, int r_index, int n, char *str)
{
	int		i;
	int		j;
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
	ret = (char *)ft_calloc(ret_len + 1, sizeof(char));
	if (!ret)
		output_err("error allocating mem for return (string\n", NULL, 0);
	i = 0;
	while (i < r_index)
	{
		ret[i] = src[i];
		i++;
	}
	j = 0;
	while (str[j])
	{
		ret[i + j] = str[j];
		j++;
	}
	i = i + j;
	while (i < ret_len && src[last_index])
		ret[i++] = src[last_index++];
	ret[i] = 0;
	free(str);
	free(src);
	return (ret);
}

char	*ft_wildcard(char *str)
{
	int		i;
	int		start_index;
	int		end_index;
	int		len;
	char	*ret;
	char	*tmp;

	i = 0;
	start_index = 0;
	end_index = 0;
	len = 0;
	ret = ft_strdup(str);
	tmp = NULL;
	while (ret[i])
	{
		if (ret[i] == '*')
		{
			start_index = get_start_index(ret, i);
			end_index = get_end_index(ret, i);
			tmp = get_wildcard(ft_str_extract(ret + start_index,
												end_index - start_index));
			i = (i - (end_index - start_index) + (end_index - i))
				+ ft_strlen(tmp);
			ret = str_replace_free(ret, start_index, end_index - start_index
					+ 1, tmp);
		}
		else
			i++;
	}
	free(str);
	return (ret);
}

void	set_wc(t_data *data)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = *data->cmd_list;
	while (cmd)
	{
		if (cmd->type == CMD)
		{
			i = -1;
			while (cmd->args[++i])
				cmd->args[i] = ft_wildcard(cmd->args[i]);
		}
		cmd = cmd->next;
	}
}
