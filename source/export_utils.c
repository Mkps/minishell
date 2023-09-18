/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:50:33 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/18 16:35:22 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_lstadd_back_env(t_env **lst, t_env *new)
{
    if (!lst || !new)
        return;
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

t_env *ft_lstnew_env(char *key, char *value)
{
    t_env *new_env = (t_env *)malloc(sizeof(t_export));
    if (!new_env)
        return NULL;
    new_env->key = ft_strdup(key);
    new_env->value = ft_strdup(value);
    new_env->next = NULL;

    return new_env;
}

void free_env_list(t_env *env)
{
    t_env *current = env;
    while (current != NULL)
    {
        t_env *next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}

int env_key_exists(t_env *env, char *key_to_check)
{
    while (env != NULL)
    {
        if (ft_strncmp(env->key, key_to_check, ft_strlen(key_to_check)) == 0)
            return 1;
        env = env->next;
    }
    return 0;
}

int ft_lstsize_env(t_env *lst)
{
    size_t size;

    size = 0;
    while (lst)
    {
        lst = lst->next;
        size++;
    }
    return (size);
}

void print_envp(t_data *data)
{
    int i = 0;

    while (data->envv[i])
    {
        printf("%s\n", data->envv[i]);
        i++;
    }
}