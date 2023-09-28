/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:10:34 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/28 18:26:13 by uaupetit         ###   ########.fr       */
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
	pwd = getcwd(NULL, 0);
	tmp = ft_strjoin("PWD=", pwd);
	old_pwd = NULL;
	if (cmd->args[1] && cmd->args[2] != NULL)
		return (output_err_ret(1,
				"minishell: cd: too many arguments", NULL));
	handle_directory_change(data, &old_pwd, dir);
	if (set_pwd(pwd) == 1)
	{
		free(pwd);
		return (EXIT_FAILURE);
	}
	ft_cd_next(pwd, tmp, data, old_pwd);
	return (EXIT_SUCCESS);
}

void	handle_directory_change(t_data *data, char **old_pwd, char *dir)
{
	if (ft_strncmp(dir, "~", ft_strlen(dir)) == 0)
		handle_home_directory(data, dir);
	else if (ft_strncmp(dir, "-", ft_strlen(dir)) == 0)
		handle_previous_directory(data, old_pwd);
	else if (ft_strncmp(dir, "..", ft_strlen(dir)) == 0)
		handle_parent_directory();
	else
		handle_regular_directory(dir);
}

void	update_pwd_and_oldpwd(t_data *data, char *pwd, char *temp)
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
}

void	handle_regular_directory(char *dir)
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
	full_path = (char *)malloc(full_path_len + 1);
	if (full_path == NULL)
	{
		perror("malloc");
		return ;
	}
	ft_strlcpy(full_path, home_dir, full_path_len + 1);
	ft_strlcat(full_path, dir + 1, full_path_len + 1);
	if (chdir(full_path) != 0)
		perror("cd");
	free(full_path);
}
