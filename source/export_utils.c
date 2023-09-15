/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:50:33 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/15 11:46:26 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_lstadd_back_export(t_export **lst, t_export *new)
{
    if (!lst || !new)
        return;
    new->next = NULL;
    if (*lst)
    {
        t_export *temp = *lst;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }
    else
        *lst = new;
}

t_export *ft_lstnew_export(char *key, char *value)
{
    t_export *new_export = (t_export *)malloc(sizeof(t_export));
    if (!new_export)
        return NULL;
    new_export->export = ft_strdup("export");
    new_export->key = ft_strdup(key);
    new_export->value = ft_strdup(value);
    new_export->next = NULL;

    return new_export;
}

void free_export_list(t_export *export_lst)
{
    t_export *current = export_lst;
    while (current != NULL)
    {
        t_export *next = current->next;
        free(current->key);
        free(current->value);
        free(current->export);
        free(current);
        current = next;
    }
}

int    ft_wordsize(char *s, char c, int pos)
{
    int    wsize;

    wsize = 0;
    while (s[pos] != c && s[pos])
    {
        wsize++;
        pos++;
    }
    return (wsize);
}

char    **ft_split2(char *s, char c)
{
    int        i;
    int        j;
    int        k;
    char    **res;

    if (!s)
        return (NULL);
    k = ft_strlen(s);
    res = malloc(sizeof(char *) * 3);
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    res[0] = malloc(sizeof(char) * (ft_wordsize(s, c, j) + 1));
    if (!res[0])
        return (free(res), NULL);
    while (s[j] != c && s[j])
        res[0][i++] = s[j++];
    res[0][i] = '\0';
    i = 0;
    if (s[j] == '=')
        j++;
    res[1] = malloc(sizeof(char) * (k - j + 1));
    if (!res[1])
        return (free_tabs(res), NULL);
    while (s[j])
        res[1][i++] = s[j++];
    res[1][i] = '\0';
    res[2] = 0;
    return (res);
}

void    free_tabs(char **tab)
{
    size_t    i;

    i = 0;
    while (tab[i])
    {
        if (tab[i])
            free(tab[i]);
        i++;
    }
    if (tab)
        free(tab);
}

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

char *add_quotes(char *str) {
    size_t len = ft_strlen(str);
    char *result = (char *)malloc(len + 3);
    if (result) {
        result[0] = '"';
        ft_strlcpy(result + 1, str, len + 2);
        result[len + 1] = '"';
        result[len + 2] = '\0';
    }
    return result;
}