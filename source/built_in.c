/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:22:15 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/11 15:30:52 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_echo(t_cmd *cmd)
{
	int i = 1;
	int len = 0;
	int flag = 0;

	//printf("cmd = %s\n", cmd->args[1]);
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
        if (len >= 2 && cmd->args[i][0] == '"' && cmd->args[i][len - 1] == '"')
        {
            ft_memmove(cmd->args[i], cmd->args[i] + 1, len - 2);
            cmd->args[i][len - 2] = '\0';
        }
        printf("%s", cmd->args[i]);
        i++;
        if (cmd->args[i] != NULL)
            printf(" ");
	}
    if (!flag)
        printf("\n");
}

void    ft_cd(t_cmd *cmd, t_data *data)
{
    char *dir = NULL;
    char *pwd = NULL;
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
        }
        i = 0;
        while (data->envv[i] != NULL)
        {
            if (ft_strncmp(data->envv[i], "PWD=", 4) == 0)
            {
                free(data->envv[i]);
                data->envv[i] = ft_strjoin("PWD=", pwd);
                break;
            }
            i++;
        }
        free(pwd);     
    }
}

void ft_pwd(t_data *data)
{
    char *pwd_value = NULL;
    int i = 0;
    
    while (data->envv[i] != NULL)
    {
        if (ft_strncmp(data->envv[i], "PWD=", 4) == 0)
        {
            pwd_value = data->envv[i] + 4;
            break;
        }
        i++;
    }
    if (pwd_value == NULL) {
        fprintf(stderr, "PWD variable not found in envv\n");
        return;
    }
    printf("%s\n", pwd_value);
}

/*
void    ft_pwd_bis()
{
        char *buffer;
        size_t size = 1024;
        
        buffer = (char *)malloc(size);
        if (buffer == NULL)
        {
            perror("malloc");
            return;
        }
        if (getcwd(buffer, size) == NULL)
        {
            perror("getcwd");
            free(buffer);
            return;
        }
        printf("%s\n", buffer);
        free(buffer);
}

void	ft_cd_bis(t_cmd *cmd, t_data *data)
{
	char *dir = NULL;
    
	if (cmd->args[1] == NULL)
    {
        printf("cd: argument manquant\n");
        return;
    }
    dir = cmd->args[1];
    if (ft_strncmp(dir, "..", 2) == 0)
    {
        if (chdir("..") != 0)
            perror("cd");
        return;
    }
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
        perror("cd");
}*/