/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:46:26 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/13 10:24:32 by uaupetit         ###   ########.fr       */
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
    new_export->export = NULL;
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

t_cmd *find_export_command(t_data *data) {
    t_cmd **cmd_list = data->cmd_list; // Obtenez le tableau de commandes

    int i = 0;
    while (cmd_list && cmd_list[i]) // Assurez-vous que cmd_list et cmd_list[i] ne sont pas nuls
    {
        t_cmd *cmd = cmd_list[i];
        if (cmd->args && cmd->args[0] && ft_strncmp(cmd->args[0], "export", ft_strlen("export")) == 0) {
            return cmd; // Retourne la commande si args[0] est "export"
        }
        i++;
    }

    return NULL; // Retourne NULL si aucune commande "export" n'est trouvée
}

void    execute_export(t_data *data)
{
    int i;
    t_cmd *cmd_lst = NULL;
    
    i = 1;
    cmd_lst = find_export_command(data);
    if (cmd_lst->args[i] == NULL)
    {
        print_export(data);
        return ;
    }
    while(cmd_lst->args[i])
    {

    }
}

void    ft_export(t_data *data)
{
    env_to_export(data);
    sort_export_list(data);
    //print_export(data);
    execute_export(data);
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