/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:46:26 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/18 16:29:17 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void env_to_export(t_data *data)
{
    t_env *current = data->env_cpy;
    t_export *new_export = NULL;
    
    while (current != NULL)
    {
        new_export = ft_lstnew_export(current->key, current->value);
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


void    ft_export(t_data *data)
{
    t_cmd *cmd_lst;
    
    cmd_lst = NULL;
    if (data->export == NULL)
        env_to_export(data);
    cmd_lst = find_export_command(data);
    if (cmd_lst->args[1] == NULL)
    {
        sort_export_list(data);
        print_export(data);
        return;
    }
    else
    {
        execute_export(data, cmd_lst);
        env_update(data);
    }
}

void print_export(t_data *data)
{
    t_export *current = data->export;

    while (current != NULL)
    {   
       // if (data->flag > 0)
            printf("%s %s=%s\n", current->export, current->key, current->value);
       // else
            //printf("%s %s %s\n", current->export, current->key, current->value);
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