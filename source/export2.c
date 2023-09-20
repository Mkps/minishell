/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:19:43 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/20 12:49:06 by uaupetit         ###   ########.fr       */
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
    if (key_is_valid(key) == 1)
            return;
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

int first_char(char *str)
{
    if (str[0] == '\0')
        return 0;
    if (ft_isalpha(str[0]) || str[0] == '_' || str[0] == '=')
        return 0;
     else
        return 1;
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
    if (key_is_valid(key) == 1)
    {
        printf("export: `%s': not a valid identifier\n", key);
        return;
    }
    /*if (first_char(key) == 1 || is_valid_arg(key) == 1 || ft_equal(key) == 1)
    {
            printf("1 export: '%s': not a valid identifier\n", key);
            return;
    }
    if (is_valid_arg(value) == 1 || ft_equal(value) == 1)
    {
            printf("2 export: '%s': not a valid identifier\n", value);
            return;
    }*/
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
            return ;
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
    while (cmd->args[i])
    {
        if (ft_strlen(cmd->args[i]) == 1 && cmd->args[i][0] == '=')
        {
            printf("export: `=': not a valid identifier\n");
            i++;
        }
      //  if (is_valid_arg(cmd->args[i]) == 1)
        //    printf("export: ` ': not a valid identifier\n");
   //     else
       //     printf("good\n");
       else
        {
            set_in_export(data, cmd->args[i]);
            set_in_env(data, cmd->args[i]);
            i++;
        }
    }
}
/*
int is_valid_arg(char *arg)
{
    int i = 0;
    while (arg[i])
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '=' && arg[i] != '_' && arg[i] != '"'
            && arg[i] != ' ' && arg[i] != '\0')
            return 1;
        i++;
    }
    return 0;
}*/

int key_is_valid(char *chaine) {
    if (chaine[0] == '\0') {
        return 0; // Chaîne vide
    }

    // Vérifie si le premier caractère est une lettre ou un '_'
    if (!ft_isalpha(chaine[0]) && chaine[0] != '_') {
        return 1; // Commence par autre chose qu'une lettre ou un '_'
    }

    // Vérifie s'il y a autre chose que des caractères alphanumériques ou '_'
    for (int i = 0; chaine[i] != '\0'; i++) {
        if (!ft_isalnum(chaine[i]) && chaine[i] != '_') {
            return 1; // Contient autre chose que des caractères alphanumériques ou '_'
        }
    }

    return 0; // La chaîne est valide
}

int ft_equal(char *chaine) {
    int i = 0;
    if (ft_strlen(chaine) == 1 && chaine[0] == '=')
        return 1;
    // Recherche du signe égal '=' dans la chaîne
    while (chaine[i] != '\0') {
        if (chaine[i] == '=') {
            // Vérifie s'il y a des espaces avant et après le signe égal
            if ((i > 0 && chaine[i - 1] == ' ') && (chaine[i + 1] == ' ' || chaine[i + 1] == '\0')) {
                return 1;
            }
        }
        i++;
    }

    return 0;
}
