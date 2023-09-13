/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:46:26 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/13 16:18:11 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void env_to_export(t_data *data)
{
    t_env *current = data->env_cpy;
    
    while (current != NULL)
    {
        t_export *new_export = ft_lstnew_export(current->key, current->value);
        if (!new_export)
        {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        new_export->export = ft_strdup("export");
        ft_lstadd_back_export(&(data->export), new_export);
        current = current->next;
    }
}

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

t_cmd *find_export_command(t_data *data)
{
    t_cmd **cmd_list = data->cmd_list;
    int i = 0;
    
    while (cmd_list && cmd_list[i])
    {
        t_cmd *cmd = cmd_list[i];
        if (cmd->args && cmd->args[0] && ft_strncmp(cmd->args[0], "export", ft_strlen("export")) == 0) {
            return cmd;
        }
        i++;
    }
    return NULL;
}

void    execute_export(t_data *data, t_cmd  *cmd)
{
    int i;
  //  t_cmd *cmd_lst = NULL;
    
    i = 1;
   /* cmd_lst = find_export_command(data);
    if (cmd_lst->args[i] == NULL)
    {
        printf("********\n");
        sort_export_list(data);
        print_export(data);
        return ;
    }*/
    while(cmd->args[i])
    {
        printf("NE DOIT PAS PRINT\n");
        //check_variable;
        set_in_export(data, cmd->args[i]);
       // sort_export_list(data);
        //print_export(data);
        //set_in_env;
   //     printf("args[i] = %s\n" ,cmd_lst->args[i]);
        i++;
    }
}

void set_in_export(t_data *data, char *variable)
{
    char **variable_split = NULL;
    char *key = NULL;
    char *value = NULL;
    int i = 0;
    
    variable_split = ft_split2(variable, '=');
    key = ft_strdup(variable_split[0]);
    value = ft_strdup(variable_split[1]);

    // Créez un nouvel élément t_export pour la variable et la valeur
    t_export *new_export = ft_lstnew_export(key, value);
    if (!new_export)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }

    // Ajoutez le nouvel élément à la fin de la liste export de data
    ft_lstadd_back_export(&(data->export), new_export);
    
    // N'oubliez pas de libérer la mémoire allouée pour key et value
    free(key);
    free(value);
    while(variable_split[i])
    {
        free(variable_split[i]);
        i++;
    }
    free(variable_split);
    // N'oubliez pas de libérer la mémoire allouée pour variable_split
    // ...
}

void    ft_export(t_data *data)
{
    t_cmd *cmd_lst = NULL;

    /*free_export_list(data->export);
    data->export = NULL;*/
   
    if (data->export == NULL)
        env_to_export(data);
    cmd_lst = find_export_command(data);
   // env_to_export(data);
    if (cmd_lst->args[1] == NULL)
    {
        sort_export_list(data);
        print_export(data);
        return;
    }
    else
    {
        //sort_export_list(data);
        //print_export(data);
        execute_export(data, cmd_lst);
        // sort_export_list(data);
    }
}

void print_export(t_data *data)
{
    t_export *current = data->export;

    while (current != NULL)
    {
        printf("%s %s=%s\n", current->export, current->key, current->value);
        current = current->next;
    }
}

void sort_export_list(t_data *data)
{
    t_export *sorted = NULL;
    t_export *current = data->export;
    t_export *next;

    while (current != NULL)
    {
        next = current->next;
        insert_sorted(&sorted, current);
        current = next;
    }
    data->export = sorted;
}

void insert_sorted(t_export **sorted, t_export *new_export)
{
    t_export *current;
    
    if (*sorted == NULL || ft_strncmp(new_export->key, (*sorted)->key, ft_strlen(new_export->key)) < 0)
    {
        new_export->next = *sorted;
        *sorted = new_export;
    }
    else
    {
        current = *sorted;
        while (current->next != NULL && ft_strncmp(new_export->key, current->next->key, ft_strlen(new_export->key)) >= 0)
            current = current->next;
        new_export->next = current->next;
        current->next = new_export;
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