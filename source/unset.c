/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:42:53 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/18 15:31:35 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd *find_unset_command(t_data *data)
{
    t_cmd **cmd_list = data->cmd_list;
    int i = 0;
    
    while (cmd_list && cmd_list[i])
    {
        t_cmd *cmd = cmd_list[i];
        if (cmd->args && cmd->args[0] && ft_strncmp(cmd->args[0], "unset", ft_strlen("unset")) == 0) {
            return cmd;
        }
        i++;
    }
    return NULL;
}

void execute_unset(t_data *data, t_cmd *cmd)
{
    int i = 1;
    t_cmd *current = cmd;
    t_export *prev = NULL;
    t_export *current_export = NULL;
    
    while (cmd->args[i])
    {
        prev = NULL;
        current_export = data->export;
        
        while (current_export)
        {
            if (ft_strncmp(current_export->key, cmd->args[i], ft_strlen(cmd->args[i])) == 0)
            {
                if (prev == NULL)
                {
                    data->export = current_export->next;
                    free(current_export->key);
                    free(current_export->value);
                    free(current_export);
                }
                else
                {
                    prev->next = current_export->next;
                    free(current_export->key);
                    free(current_export->value);
                    free(current_export);
                }
                break;
            }
            prev = current_export;
            current_export = current_export->next;
        }
        i++;
    }
}

void    execute_env(t_data *data, t_cmd *cmd)
{
        int i = 1;
        t_cmd *current = cmd;
    
        while (cmd->args[i])
        {
        t_env *prev = NULL;
        t_env *current_export = data->env_cpy;
        while (current_export)
        {
            if (ft_strncmp(current_export->key, cmd->args[i], ft_strlen(cmd->args[i])) == 0)
            {
                if (prev == NULL)
                {
                    data->env_cpy = current_export->next;
                    free(current_export->key);
                    free(current_export->value);
                    free(current_export);
                }
                else
                {
                    prev->next = current_export->next;
                    free(current_export->key);
                    free(current_export->value);
                    free(current_export);
                }
                printf("Variable supprimée : %s\n", cmd->args[i]);
                break;
            }   
            prev = current_export;
            current_export = current_export->next;
        }
        i++;
    }
}

void    ft_unset(t_data *data)
{
    t_cmd   *cmd_lst;
    
    cmd_lst = NULL;
    cmd_lst = find_unset_command(data);
    if (cmd_lst->args[1] == NULL)
        return ;
    else
    {
        execute_unset(data, cmd_lst);
        execute_env(data, cmd_lst);
        env_update(data);
    }
}

void remove_export(t_data *data, const char *key_to_remove)
{
    t_export *prev = NULL;
    t_export *current_export = NULL; 
    
    current_export = data->export;
    while (current_export)
    {
        if (ft_strncmp(current_export->key, key_to_remove, ft_strlen(current_export->key)) == 0)
        {
            if (prev == NULL)
            {
                data->export = current_export->next;
            }
            else
            {
                prev->next = current_export->next;
            }
            free(current_export->key);
            free(current_export->value);
            free(current_export);
            break;
        }
        prev = current_export;
        current_export = current_export->next;
    }
}

void remove_env(t_data *data, const char *key_to_remove)
{
    t_env *prev = NULL;
    t_env *current_env = NULL; 
    
    current_env = data->env_cpy;
    while (current_env)
    {
        if (ft_strncmp(current_env->key, key_to_remove, ft_strlen(current_env->key)) == 0)
        {
            if (prev == NULL)
            {
                data->env_cpy = current_env->next;
            }
            else
            {
                prev->next = current_env->next;
            }
            free(current_env->key);
            free(current_env->value);
            free(current_env);
            break;
        }
        prev = current_env;
        current_env = current_env->next;
    }
}