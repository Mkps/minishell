/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/18 17:20:03 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

void    ft_env(t_data *data)
{
    print_envp(data);
    return ;
}

void    ft_echo(t_cmd *cmd)
{
	int i = 1;
	int len = 0;
	int flag = 0;

	if (cmd->args[1] == NULL)
	{
		printf("\n");
		return;
	}
    if (cmd->args[0] != NULL && ft_strncmp(cmd->args[1], "-n", 2) == 0)
	{
		flag = 1;
	    i++;
	}
    while (cmd->args[i] != NULL)
    {
        len = ft_strlen(cmd->args[i]);
        /*if (len >= 2 && cmd->args[i][0] == '"' && cmd->args[i][len - 1] == '"')*/
        /*{*/
        /*    ft_memmove(cmd->args[i], cmd->args[i] + 1, len - 2);*/
        /*    cmd->args[i][len - 2] = '\0';*/
        /*}*/
        printf("%s", cmd->args[i]);
        i++;
        if (cmd->args[i] != NULL)
            printf(" ");
	}
    if (!flag)
        printf("\n");
}

void    ft_cd_bis(t_cmd *cmd, t_data *data)
{
    char *dir = NULL;
    char *pwd = NULL;
    t_env *current = data->env_cpy;
    int i;

    i = 0;
    if (cmd->args[1] == NULL) {
        printf("cd: argument manquant\n");
        return;
    }
    dir = cmd->args[1];
    if (ft_strncmp(dir, "..", 2) == 0)
    {
        if (chdir("..") != 0) {
            perror("cd");
            return;
        }
    }
    else
    {
        if (dir[0] == '"' && dir[ft_strlen(dir) - 1] == '"')
        {
            ft_memmove(dir, dir + 1, ft_strlen(dir) - 2);
            dir[ft_strlen(dir) - 2] = '\0';
        }
        if (access(dir, F_OK) == -1)
        {
            perror("cd");
            return;
        }
        if (chdir(dir) != 0) {
            perror("cd");
            return;
        }
        pwd = getcwd(NULL, 0);
        if (pwd == NULL)
        {
            perror("getcwd");
            return;
        }/*
        i = 0;
        while (data->envv[i] != NULL)
        {
            if (ft_strncmp(data->envv[i], "PWD", 3) == 0)
            {
                free(data->envv[i]);
                data->envv[i] = ft_strjoin("PWD=", pwd);
                break;
            }
            i++;
        }*/
        while (current != NULL)
        {
            if (ft_strncmp(current->key, "PWD", 3) == 0)
            {
                
            }
        }
        free(pwd);     
    }
}

void ft_pwd(t_data *data)
{
    char *pwd_value = NULL;
    t_env *current = data->env_cpy;
    
    while (current != NULL)
    {
        if (ft_strncmp(current->key, "PWD", 3) == 0)
        {
            pwd_value = current->value;
            break;
        }
        current = current->next;
    }
    
    if (pwd_value == NULL)
    {
        fprintf(stderr, "PWD variable not found in envv\n");
        return;
    }
    
    printf("%s\n", pwd_value);
}

void ft_cd(t_cmd *cmd, t_data *data)
{
    char *dir = NULL;
    char *pwd = NULL;
    t_env *current = data->env_cpy;
    char *tmp = NULL;
    
    if (cmd->args[1] == NULL)
    {
        printf("cd: argument manquant\n");
        return;
    }
    dir = cmd->args[1];
    if (ft_strncmp(dir, "..", 2) == 0)
    {
        if (chdir("..") != 0) {
            perror("cd");
            return;
        }
        pwd = getcwd(NULL, 0);
        if (pwd == NULL)
        {
            perror("getcwd");
            return;
        }
    }
    else
    {
        if (dir[0] == '"' && dir[ft_strlen(dir) - 1] == '"')
        {
            ft_memmove(dir, dir + 1, ft_strlen(dir) - 2);
            dir[ft_strlen(dir) - 2] = '\0';
        }
        if (access(dir, F_OK) == -1)
        {
            perror("cd");
            return;
        }
        if (chdir(dir) != 0)
        {
            perror("cd");
            return;
        }
        pwd = getcwd(NULL, 0);
        if (pwd == NULL)
        {
            perror("getcwd");
            return;
        }
        while (current != NULL)
        {
            if (ft_strncmp(current->key, "PWD", 3) == 0)
            {
                free(current->value);
                current->value = ft_strdup(pwd);
                break;
            }
            current = current->next;
        }
    }
	tmp = ft_strjoin("PWD=", pwd);
    // printf("%s %s\n", tmp, current->value);
	ft_setenv(data, tmp);
    free(pwd);
}

