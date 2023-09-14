#include "../include/minishell.h"

//Creates a new token then adds it to the end of the list.
t_token	*ft_new_token(t_token **root, char *value, int type)
{
	t_token *t;

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
t_token *last_token(t_token **root)
{
	t_token *current;

	current = *root;
	if (!current)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	add_token_back(t_token **root, int type, char *value)
{
	t_token *current;

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

// Token constructor with type and value
t_token	*create_token(int type, char *value)
{
	t_token	*ret;

	ret = malloc(sizeof(t_token));
	ret->next = NULL;
	ret->prev = NULL;
	if (value == NULL)
		ret->value = "";
	else
		ret->value = value;
	ret->raw_value = value;
	ret->token_type = type;
	ret->near_quote = 0;
	ret->quote_status = NONE;
	return (ret);
}
