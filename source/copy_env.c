/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:59:12 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/21 17:02:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void copy_env_to_list(t_data *data)
{
    int i = 0;
    char *equal_sign = NULL;
    char *key = NULL;
    char *value = NULL;
    t_env *new_env;
    
    while (data->envv[i])
    {
        equal_sign = ft_strrchr(data->envv[i], '=');
        if (equal_sign != NULL)
        {
            key = ft_substr(data->envv[i], 0, equal_sign - data->envv[i]);
            value = ft_strdup(equal_sign + 1);
            if (key && value)
            {
                new_env = ft_lstnew_two(key, value);
                if (new_env)
				{
                    ft_lstadd_back_two(data->env_cpy, new_env); // Utilisez data->env_cpy
				}
				else
                {
					free(key);
					free(value);
					printf("ERROR failed to initiallize new_env \n");
                }
            }
        }
        i++;
    }
}

void free_env_lst(t_env **env_lst)
{
	t_env *current = *env_lst;
	t_env	*next;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void ft_lstadd_back_two(t_env **lst, t_env *new)
{
	if (!lst || !new)
		return ;
	new->next = NULL;
	if (*lst)
	{
		t_env *temp = *lst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	else
		*lst = new;
}

t_env	*ft_lstnew_two(char *key, char *value)
{
	t_env *list = (t_env *)malloc(sizeof(t_env));
	if (!list)
		return NULL;
	list->key = key;
	list->value = value;
	list->next = NULL;
	return list;
}

void print_env_list(t_env **env_lst)
{
    t_env *current;

    current = *env_lst;
    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}
