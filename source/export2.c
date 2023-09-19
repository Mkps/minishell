/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:19:43 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/19 16:52:42 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    env_update(t_data *data)
{
    int num_env = 0;
    int i = 0;
    t_env *current_env = NULL;

    if (data->envv)
    {
        i = 0;
        while (data->envv[i])
        {
            free(data->envv[i]);
            i++;
        }
        free(data->envv);
    }
    num_env = ft_lstsize_env(data->env_cpy);
    data->envv = (char **)malloc((num_env + 1) * sizeof(char *));
    if (!data->envv)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    current_env = data->env_cpy;
    i = 0;
    while (current_env)
    {
        size_t key_len = ft_strlen(current_env->key);
        size_t value_len = ft_strlen(current_env->value);
        size_t entry_len = key_len + value_len + 2;
        data->envv[i] = (char *)malloc(entry_len);
        if (!data->envv[i])
        {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        ft_strlcpy(data->envv[i], current_env->key, entry_len);
        ft_strlcat(data->envv[i], "=", entry_len);
        ft_strlcat(data->envv[i], current_env->value, entry_len);
        current_env = current_env->next;
        i++;
    }
    data->envv[i] = NULL;
}

void    set_in_env(t_data *data, char *variable)
{
    char **variable_split = NULL;
    char *key = NULL;
    char *value = NULL;
    t_env *new_env = NULL;
    int i = 0;

    if (data->flag > 0)
        return ;
    
    variable_split = ft_split2(variable, '=');
    key = ft_strdup(variable_split[0]);
    value = ft_strdup(variable_split[1]);
    if (value[0] == '\0')
        return ;
   
   // if (ft_strrchr(variable, '=') == NULL)
     //   return ;
    new_env = ft_lstnew_env(key, value);
    if (!new_env)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    if (env_key_exists(data->env_cpy, key) == 1)
    {
        printf("\n key existe deja \n");
        if (value[0] != '\0')
            remove_env(data, key);
        else
            return;
    }
    ft_lstadd_back_env(&(data->env_cpy), new_env);
    free(key);
    free(value);
    while (variable_split[i])
    {
        free(variable_split[i]);
        i++;
    }
    free(variable_split);
}

void    set_in_export(t_data *data, char *variable)
{
    char **variable_split = NULL;
    char *key = NULL;
    char *value = NULL;
    t_export *new_export = NULL;
    int i = 0;
    int flag = 0;

    variable_split = ft_split2(variable, '=');
    key = ft_strdup(variable_split[0]);
    value = ft_strdup(variable_split[1]);
  //  if (variable_split[1] == NULL || variable_split[0][0] != '\0')
    if (ft_strrchr(variable, '=') == NULL)
        flag++;
   // printf("FLAG= %i\n", flag);
    if (value[0] != '\0')
        value = add_quotes(value);
    if (export_key_exists(data->export, key) == 1)
    {
        if (value[0] != '\0')
            remove_export(data, key);
        else
            return;
    }
    new_export = ft_lstnew_export(key, value, flag);
    if (!new_export)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    ft_lstadd_back_export(&(data->export), new_export);
    free(key);
    if (value)
        free(value);
    while (variable_split[i])
    {
        free(variable_split[i]);
        i++;
    }
    free(variable_split);
}

void    execute_export(t_data *data, t_cmd *cmd)
{
    int i = 1;
    int flag = 0;
   /* if (is_valid_export(cmd->args[i]) == 1)
    {
        printf("L'export n'est pas valide");
        return;
    }*/
    while (cmd->args[i])
    {
        /*if (fonction a coder(cmd->args[i][0]) == 1 && flag == 0)
        {
            printf("export: `=': not a valid identifier");
            flag++;
        }*/
        set_in_export(data, cmd->args[i]);
        set_in_env(data, cmd->args[i]);
        i++;
    }
}

int is_valid_export(char *str)
{
    int i = 0;
    int equal_found = 0;  // Variable pour suivre si le signe égal a été trouvé

    // Chercher le premier signe égal '=' dans la chaîne
    while (str[i] != '\0') {
        if (str[i] == '=') {
            equal_found = 1;
            break;  // Sortir de la boucle dès que le signe égal est trouvé
        }
        i++;
    }

    // Si le signe égal n'a pas été trouvé ou s'il est précédé ou suivi d'un espace, alors c'est invalide.
    if (!equal_found || (i > 0 && str[i - 1] == ' ') || str[i + 1] == ' ') {
        return 1;
    }

    return 0;
}
