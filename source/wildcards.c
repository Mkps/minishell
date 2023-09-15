/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:19:35 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/14 18:13:41 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

typedef struct s_wcnode{
	struct s_wcnode	*next;
	struct s_wcnode	*prev;
	int				level;
	char			**refs;
}	t_wcnode;

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
t_wcnode	*get_last_node(t_wcnode **root)
{
	t_wcnode	*tmp;

	tmp = *root;
	while (tmp)
		tmp = tmp->next;
	return (tmp);
}

void	add_node(t_wcnode **root)
{
	t_wcnode	*last;

	last = get_last_node(root);
	if (*root == NULL)
		root = malloc(sizeof (t_wcnode));
	else
		get_last_node(root)->next = malloc(sizeof (t_wcnode));
	get_last_node(root)->next = NULL;
}

char	*str_tolower(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return (str);
}

int		ft_strcmp_no_case(char *s1, char *s2)
{
	char	*tmp_s1;
	char	*tmp_s2;
	int		ret;

	tmp_s1 = ft_strdup(s1);
	tmp_s2 = ft_strdup(s2);
	ret = strcmp(str_tolower(tmp_s1), str_tolower(tmp_s2));
	free(tmp_s1);
	free(tmp_s2);
	return (ret);
}
char	*ft_strjoin_tab(char **tab)
{
	int	i;
	char *ret;

	i = 1;
	if (!tab[0])
		return (NULL);
	if (tab[0] && !tab[0 + i])
		return (tab[i]);
	tab[0] = ft_strappend(tab[0], " ", 2);
	while (tab[0] && tab[0 + i])
	{
		tab[0] = ft_strappend(tab[0], tab[0 + i], 3);
		tab[0] = ft_strappend(tab[0], " ", 2);
		i++;
	}
	ret = ft_strdup(tab[0]);
	free(tab[0]);
	free(tab);
	return (ret);

}
void	ft_str_swap(char **s1, char **s2)
{
	char *tmp;

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
	free(str);
	i = 0;
	while (split[i] && split[i + 1])
	{
		if (ft_strcmp_no_case(split[i], split[i + 1]) >= 0)
		{
			ft_str_swap(&split[i], &split[i + 1]);
			i = 0;
		}
		else
			i++;
	}
	i = 0;
	ret = ft_strjoin_tab(split);
	return (ret);
}
char	*wc_expand(t_wcnode **root)
{
   // t_wcnode	*current;
	char		*str;
	char		*ret;
	DIR			*d;
	struct dirent	*dir;

	(void)root;
	str = ft_strdup("");
	d = opendir(".");
	if (d) {
		while ((dir = readdir(d)) != NULL)
		{
			str = ft_strappend(str, dir->d_name,2);
			str = ft_strappend(str, " ", 2);
		}
		closedir(d);
	}
	ret = sort_str(str);
	return (ret);
}

char	*get_wildcard(char *str)
{
	int	i;
	int	start_index;
	int	end_index;
	t_wcnode	**root;

	// root = ft_calloc(1, sizeof(t_wcnode*));
	free(str);
	root = NULL;
	return (wc_expand(root));
}

// Replaces the string at r_index of length n by str
char	*str_replace_free(char *src, int r_index, int n, char *str)
{
	int	i;
	int	j;
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
		printf("error allocating mem for return string\n");
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
	int			i;
	int			start_index;
	int			end_index;
	int			len;
	char		*ret;
	char		*tmp;
	t_wcnode	**root;

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
			tmp = get_wildcard(ft_str_extract(ret + start_index, end_index - start_index));
			i = (i - (end_index - start_index) + (end_index - i)) + ft_strlen(tmp);
			ret = str_replace_free(ret, start_index, end_index - start_index + 1, tmp);
		}
		else
			i++;
	}
	free(str);
	return (ret);
}
