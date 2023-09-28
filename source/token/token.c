/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:42:00 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:47:14 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//Creates a new token then adds it to the end of the list.
t_token	*ft_new_token(t_token **root, char *value, int type)
{
	t_token	*t;

	t = *root;
	while (t != NULL)
		t = t->next;
	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->value = value;
	t->token_type = type;
	return (t);
}

// Returns a pointer to the last token.
t_token	*last_token(t_token **root)
{
	t_token	*current;

	current = *root;
	if (!current)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	add_token_back(t_token **root, int type, char *value)
{
	t_token	*current;

	if (*root == NULL)
	{
		*root = create_token(type, value);
		return ;
	}
	current = *root;
	while (current->next != NULL)
		current = current->next;
	current->next = create_token(type, value);
	last_token(root)->prev = current;
}

void	insert_token_next(t_token *i_p, int type, char *value)
{
	t_token	*tmp;

	if (i_p == NULL)
	{
		i_p = create_token(type, value);
		return ;
	}
	tmp = i_p->next;
	i_p->next = create_token(type, value);
	if (tmp)
	{
		i_p->next->prev = i_p;
		i_p->next->next = tmp;
		tmp->prev = i_p->next;
	}
	else
	{
		i_p->next->prev = i_p;
		i_p->next->next = NULL;
	}
}

// Token constructor with type and value
t_token	*create_token(int type, char *value)
{
	t_token	*ret;

	ret = malloc(sizeof(t_token));
	if (!ret)
		return (NULL);
	ret->next = NULL;
	ret->prev = NULL;
	ret->value = value;
	ret->raw_value = value;
	ret->token_type = type;
	ret->near_quote = 0;
	ret->quote_status = NONE;
	return (ret);
}
