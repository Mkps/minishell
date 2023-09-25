/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:10:34 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/25 10:55:10 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	*dir;
	char	*pwd;
	t_env	*current;
	char	*tmp;
	char	*old_pwd;

	dir = NULL;
	pwd = NULL;
	current = *data->env_cpy;
	tmp = NULL;
	old_pwd = NULL; //getcwd(NULL, 0);
	if (cmd->args[2] != NULL)
		return (output_err_ret(1, "minishell: cd: too many arguments\n");
	if (cmd->args[1] == NULL)
		return (output_err_ret(1, "minishell: cd: need absolute or relative path\n"));
	dir = cmd->args[1];
	if (ft_strncmp(dir, "~", ft_strlen(dir)) == 0)
	{
		handle_home_directory(data, dir);
	}
	else if (ft_strncmp(dir, "-", ft_strlen(dir)) == 0)
	{
		handle_previous_directory(data, &old_pwd);
	}
	else if (ft_strncmp(dir, "..", ft_strlen(dir)) == 0)
	{
		handle_parent_directory();
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
			printf("minishell: cd: %s: No such file or directory\n", dir);
		}
		else
		{
			if (chdir(dir) != 0)
			{
				perror("cd");
			}
		}
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("getcwd");
		return ;
	}
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
	current = *data->env_cpy;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, "OLDPWD", 6) == 0)
		{
			free(current->value);
			current->value = ft_strdup(pwd);
			break ;
		}
		current = current->next;
	}
	tmp = ft_strjoin("PWD=", pwd);
	ft_setenv(data, tmp);
	free(pwd);
	return (EXIT_SUCCESS);
}

void	handle_parent_directory(void)
{
	if (chdir("..") != 0)
	{
		perror("cd");
	}
}

void	handle_previous_directory(t_data *data, char **old_pwd)
{
	char	*old_pwd_env;

	old_pwd_env = ft_getenv(data->envv, "OLDPWD");
	if (old_pwd_env == NULL)
	{
		printf("cd: OLDPWD not set\n");
		return ;
	}
	// Allouez de la mémoire pour old_pwd et copiez la valeur de old_pwd_env.
	*old_pwd = ft_strdup(old_pwd_env);
	if (chdir(old_pwd_env) != 0)
	{
		perror("cd");
		free(*old_pwd); // Assurez-vous de libérer la mémoire en cas d'erreur.
	}
}

void	handle_home_directory(t_data *data, const char *dir)
{
	char *home_dir = ft_getenv(data->envv, "HOME");
	if (home_dir == NULL)
	{
		printf("cd: HOME not set\n");
		return ;
	}
	size_t full_path_len = strlen(home_dir) + strlen(dir) - 1;
	char full_path[full_path_len + 1]; // +1 for null terminator
	ft_strlcpy(full_path, home_dir, sizeof(full_path));
	ft_strlcat(full_path, dir + 1, sizeof(full_path));
	if (chdir(full_path) != 0)
	{
		perror("cd");
	}
}
