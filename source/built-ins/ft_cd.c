/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:10:34 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/03 15:07:21 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_cmd *cmd, t_data *data)
{	
	char	*dir;
	char	*pwd;
	char	*tmp;
	char	*old_pwd;

	if (cmd->args[1] && cmd->args[1][0])
		dir = cmd->args[1];
	else
		dir = "~";
	if (ft_strncmp(dir, ".", 2) == 0)
		return (EXIT_SUCCESS);
	if (cmd->args[1] && cmd->args[2] != NULL)
		return (output_err_ret(1,
				"cd: too many arguments", NULL));
	pwd = getcwd(NULL, 0);
	if (set_pwd(pwd) == 1)
	{
		free(pwd);
		return (EXIT_FAILURE);
	}
	tmp = ft_strjoin("PWD=", pwd);
	old_pwd = NULL;
	handle_directory_change(data, &old_pwd, dir);
	ft_cd_next(pwd, tmp, data, old_pwd);
	return (EXIT_SUCCESS);
}

void	ft_cd_next(char *pwd, char *tmp, t_data *data, char *old_pwd)
{
	char	*temp;

	(void)tmp;
	if (pwd)
		free(pwd);
	pwd = getcwd(NULL, 0);
	temp = old_pwd;
	update_pwd_and_oldpwd(data, pwd, temp);
	free(pwd);
	free(old_pwd);
}

void	cd_env_update(t_data *data, size_t i, size_t env_count)
{
	size_t		key_len;
	size_t		value_len;
	size_t		len;
	t_env		*current;

	current = *data->env_cpy;
	if (data->envv)
		free(data->envv);
	data->envv = (char **)malloc((env_count + 1) * sizeof(char *));
	if (data->envv == NULL)
	{
		printf("cd_env_update: Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	while (current != NULL)
	{
		key_len = strlen(current->key);
		value_len = strlen(current->value);
		len = key_len + value_len + 2;
		data->envv[i] = (char *)malloc(len);
		if (data->envv[i] == NULL)
		{
			printf("cd_env_update: Malloc failed\n");
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(data->envv[i], current->key, key_len + 1);
		data->envv[i][key_len] = '=';
		ft_strlcpy(data->envv[i] + key_len + 1, current->value, value_len + 1);
		i++;
		current = current->next;
	}
	data->envv[env_count] = NULL;
}

void	update_pwd(t_data *data, char *pwd)
{
	t_env	*current;

	temp = pwd;
	current = *data->env_cpy;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, "PWD", 3) == 0)
		{
			free(current->value);
			current->value = ft_strdup(pwd);
			break ;
		}
		current = current->next;
	}
}

void	update_pwd_and_oldpwd(t_data *data, char *pwd, char *old_pwd)
{
	t_env		*current;
	size_t		env_count;
	size_t		i;

	env_count = ft_lstsize_env(data->env_cpy);
	i = 0;
	if (pwd)
		update_pwd(data, pwd);
	if (!old_pwd)
		old_pwd = "";
	current = *data->env_cpy;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, "OLDPWD", 6) == 0)
		{
			free(current->value);
			current->value = ft_strdup(temp);
			break ;
		}
		current = current->next;
	}
	cd_env_update(data, i, env_count);
}
