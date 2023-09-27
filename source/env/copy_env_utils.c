#include "../../include/minishell.h"

void	free_env_lst(t_env **env_lst)
{
	t_env	*current;
	t_env	*next;

	current = *env_lst;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	ft_lstadd_back_two(t_env **lst, t_env *new)
{
	t_env	*temp;

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

t_env	*ft_lstnew_two(char *key, char *value)
{
	t_env	*list;

	list = (t_env *)malloc(sizeof (t_env));
	if (!list)
		return (NULL);
	list->key = key;
	list->value = value;
	list->next = NULL;
	return (list);
}
