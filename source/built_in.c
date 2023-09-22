/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/22 17:13:10 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../include/minishell.h"

void	ft_env(t_data *data)
{
	print_envp(data);
	return ;
}

void	ft_echo(t_cmd *cmd)
{
	int i = 1;
	int len = 0;
	int flag = 0;
	int j = 1;
    
   // printf("cmd = |%s|.| |\n", cmd->args[1]);
	if (cmd->args[1] == NULL)
	{
		printf("\n");
		return;
	}
	if (cmd->args[i][0] == '-')
	{
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] == '\0')
		{
			flag++;
			i++;
		}
	}
	while (cmd->args[i][0] == '-')
	{
		j = 1;
		while (cmd->args[i][j] && cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] == '\0')
			i++;
		else
			break; 
	}
	while (cmd->args[i] != NULL)
	{
		printf("%s", cmd->args[i]);
		i++;
		if (cmd->args[i] != NULL)
			printf(" ");
        else
            break;
	}
	if (!flag)
		printf("\n");
}

void ft_pwd(t_data *data)
{
	char *pwd_value = NULL;
	t_env *current = *data->env_cpy;
	
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