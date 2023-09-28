/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:10:34 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/27 18:45:47 by aloubier         ###   ########.fr       */
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

	current = *data->env_cpy;
	old_pwd = "";
	if (cmd->args[2] != NULL)
		return (output_err_ret(1, "minishell: cd: too many arguments\n", NULL));
	if (cmd->args[1] == NULL)
		return (output_err_ret(1,
				"minishell: cd: need absolute or relative path\n", NULL));
	dir = cmd->args[1];
	if (ft_strncmp(dir, "~", ft_strlen(dir)) == 0)
		handle_home_directory(data, dir);
	else if (ft_strncmp(dir, "-", ft_strlen(dir)) == 0)
		handle_previous_directory(data, &old_pwd);
	else if (ft_strncmp(dir, "..", ft_strlen(dir)) == 0)
		handle_parent_directory();
	else
	{
		if (dir[0] == '"' && dir[ft_strlen(dir) - 1] == '"')
		{
			ft_memmove(dir, dir + 1, ft_strlen(dir) - 2);
			dir[ft_strlen(dir) - 2] = '\0';
		}
		if (access(dir, F_OK) == -1)
			printf("minishell: cd: %s: No such file or directory\n", dir);
		else
		{
			if (chdir(dir) != 0)
				perror("cd");
		}
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("getcwd");
		return (EXIT_FAILURE);
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
	*old_pwd = ft_strdup(old_pwd_env);
	if (chdir(old_pwd_env) != 0)
	{
		perror("cd");
		free(*old_pwd);
	}
}

void	handle_home_directory(t_data *data, const char *dir)
{
	char	*home_dir;
	size_t	full_path_len;
	char	*full_path;

	home_dir = ft_getenv(data->envv, "HOME");
	if (home_dir == NULL)
	{
		printf("cd: HOME not set\n");
		return ;
	}
	full_path_len = strlen(home_dir) + strlen(dir) - 1;
	full_path = (char *)ft_calloc(full_path_len, sizeof (char));
	ft_strlcpy(full_path, home_dir, sizeof(full_path));
	ft_strlcat(full_path, dir + 1, sizeof(full_path));
	if (chdir(full_path) != 0)
	{
		perror("cd");
	}
	free(full_path);
}
