/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:46:26 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/15 13:51:41 by uaupetit         ###   ########.fr       */
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
    
    i = 1;
    while(cmd->args[i])
    {
        set_in_export(data, cmd->args[i]);
        set_in_env(data, cmd->args[i]);
        i++;
    }
}

void set_in_env(t_data *data, char *variable)
{
    char **variable_split = NULL;
    char *key = NULL;
    char *value = NULL;
    t_env    *new_env = NULL;
    int i = 0;
    
    variable_split = ft_split2(variable, '=');
    key = ft_strdup(variable_split[0]);
    value = ft_strdup(variable_split[1]);
    new_env = ft_lstnew_env(key, value);
    if (!new_env)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    ft_lstadd_back_env(&(data->env_cpy), new_env);
    free(key);
    free(value);
    while(variable_split[i])
    {
        free(variable_split[i]);
        i++;
    }
    free(variable_split);
}

void set_in_export(t_data *data, char *variable)
{
    char **variable_split = NULL;
    char *key = NULL;
    char *value = NULL;
    t_export    *new_export = NULL;
    int i = 0;
    
    variable_split = ft_split2(variable, '=');
    key = ft_strdup(variable_split[0]);
 //   if (ft_strrchr(variable, '=') != NULL)
   // {   
        value = ft_strdup(variable_split[1]);
        if (value[0] != '\0')
        {
            value = add_quotes(value);
            data->flag++;
        }   
    //}
   /* if (ft_strrchr(variable, '=') == NULL)
        new_export = ft_lstnew_export(key, NULL);*/    
    new_export = ft_lstnew_export(key, value);
    if (!new_export)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    ft_lstadd_back_export(&(data->export), new_export);
    free(key);
    if (value)
        free(value);
    while(variable_split[i])
    {
        free(variable_split[i]);
        i++;
    }
    free(variable_split);
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
        execute_export(data, cmd_lst);
}

void print_export(t_data *data)
{
    t_export *current = data->export;

    while (current != NULL)
    {
        //if (current->value != NULL)
            printf("%s %s=%s\n", current->export, current->key, current->value);
        //else
          //  printf("%s %s %s\n", current->export, current->key, current->value);
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