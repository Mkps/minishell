/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:42:53 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/15 15:50:27 by uaupetit         ###   ########.fr       */
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
/*
void    execute_unset_bis(t_data *data, t_cmd *cmd)
{
    int i = 1;
    t_cmd *current = cmd;
    
    while(cmd->args[i])
    {
        //on parcourt la liste chainee export jusqu a trouver celle qui est = a args[i]
        //si on trouve pas on incremente i et si on trouve on printf("lst a clearer") puis on incremente i
        if (ft_strncmp(data->export->key, cmd->args[i], ft_strlen(cmd->arg) != 0)
        {
            
        }
        i++;
    }
}*/

void execute_unset(t_data *data, t_cmd *cmd)
{
    int i = 1;
    t_cmd *current = cmd;
    
    while (cmd->args[i])
    {
        // Parcourir la liste chaînée d'export jusqu'à trouver celle qui a une clé (key) égale à args[i].
        t_export *prev = NULL;
        t_export *current_export = data->export;
        while (current_export)
        {
            if (ft_strncmp(current_export->key, cmd->args[i], ft_strlen(cmd->args[i])) == 0)
            {
                // Correspondance trouvée, supprimer la variable d'environnement.
                if (prev == NULL)
                {
                    // Supprimer le premier élément de la liste.
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
                printf("Variable supprimée : %s\n", cmd->args[i]);
                break; // Sortir de la boucle while.
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
        // Parcourir la liste chaînée d'export jusqu'à trouver celle qui a une clé (key) égale à args[i].
        t_env *prev = NULL;
        t_env *current_export = data->env_cpy;
        while (current_export)
        {
            if (ft_strncmp(current_export->key, cmd->args[i], ft_strlen(cmd->args[i])) == 0)
            {
                // Correspondance trouvée, supprimer la variable d'environnement.
                if (prev == NULL)
                {
                    // Supprimer le premier élément de la liste.
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
                break; // Sortir de la boucle while.
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
    {
//        printf("error(temp)");
        return ;
    }
    else
    {
        execute_unset(data, cmd_lst);
        execute_env(data, cmd_lst);
    }
}