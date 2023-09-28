/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:23:33 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/28 17:24:10 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	insert_sorted(t_export **sorted, t_export *new_export)
{
	t_export	*current;

	if (*sorted == NULL || ft_strncmp(new_export->key, (*sorted)->key,
			ft_strlen(new_export->key)) < 0)
	{
		new_export->next = *sorted;
		*sorted = new_export;
	}
	else
	{
		current = *sorted;
		while (current->next != NULL && ft_strncmp(new_export->key,
				current->next->key, ft_strlen(new_export->key)) >= 0)
			current = current->next;
		new_export->next = current->next;
		current->next = new_export;
	}
}

char	**ft_split2(char *s, char c)
{
	int		i;
	int		j;
	char	**res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	res[0] = ft_calloc(ft_wordsize(s, c, j) + 1, sizeof(char));
	if (!res[0])
		return (free(res), NULL);
	while (s[j] != c && s[j])
		res[0][i++] = s[j++];
	i = 0;
	if (s[j] == '=')
		j++;
	res[1] = ft_strdup(s + j);
	if (!res[1])
		return (ft_free_tab(res), NULL);
	res[2] = 0;
	return (res);
}

void	ft_lstadd_back_export(t_export **lst, t_export *new)
{
	t_export	*temp;

	if (!lst || !new)
		return ;
	new->next = NULL;
	if (*lst)
	{
		temp = *lst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	else
		*lst = new;
}

t_export	*ft_lstnew_export(char *key, char *value, int flag)
{
	t_export	*new_export;

	new_export = (t_export *)malloc(sizeof(t_export));
	if (!new_export)
		return (NULL);
	new_export->key = ft_strdup(key);
	new_export->value = ft_strdup(value);
	new_export->flag = flag;
	new_export->next = NULL;
	return (new_export);
}

void	free_export_list(t_export **export_lst)
{
	t_export	*current;
	t_export	*next;

	current = *export_lst;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		current->flag = 0;
		free(current);
		current = next;
	}
}
