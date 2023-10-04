/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_chdir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:17:41 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 13:59:27 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_regular_directory(char *dir, t_data *data)
{
	if (access(dir, F_OK) == -1)
	{
		data->dir_flag = 1;
		printf("minishell: cd: %s: No such file or directory\n", dir);
		return (1);
	}
	else if (chdir(dir) != 0)
	{
		output_err_cmd(strerror(errno), "cd");
		return (1);
	}
	return (0);
}

int	handle_home_directory(t_data *data, const char *dir, size_t full_path_len)
{
	char	*home_dir;
	char	*full_path;

	home_dir = ft_getenv(data->envv, "HOME");
	if (home_dir == NULL)
	{
		printf("minishell: cd: HOME not set\n");
		return (1);
	}
	full_path_len = strlen(home_dir) + strlen(dir) - 1;
	full_path = (char *)malloc(full_path_len + 1);
	if (full_path == NULL)
	{
		output_err_cmd(strerror(errno), "cd: malloc:");
		return (1);
	}
	ft_strlcpy(full_path, home_dir, full_path_len + 1);
	ft_strlcat(full_path, dir + 1, full_path_len + 1);
	if (chdir(full_path) != 0)
	{
		output_err_cmd(strerror(errno), "cd");
		return (1);
	}
	free(full_path);
	return (0);
}

int	handle_previous_directory(t_data *data, char **old_pwd)
{
	char	*old_pwd_env;

	(void)old_pwd;
	old_pwd_env = ft_getenvcpy(data, "OLDPWD");
	if (old_pwd_env == NULL)
	{
		output_err_cmd("OLDPWD not set", "cd");
		return (1);
	}
	printf("old pwd = %s\n", old_pwd_env);
	if (chdir(old_pwd_env) != 0)
	{
		output_err_cmd(strerror(errno), "cd");
		return (1);
	}
	return (0);
}

int	handle_parent_directory(void)
{
	if (chdir("..") != 0)
	{
		return (1);
		output_err_cmd(strerror(errno), "cd");
	}
	return (0);
}

int	handle_directory_change(t_data *data, char **old_pwd, char *dir)
{
	char	*current_dir;
	size_t	full_path_len;

	full_path_len = 0;
	current_dir = getcwd(NULL, 0);
	if (*old_pwd)
		free(*old_pwd);
	*old_pwd = current_dir;
	if (ft_strncmp(dir, "~", ft_strlen(dir)) == 0)
	{
		if (handle_home_directory(data, dir, full_path_len) == 1)
			return (1);
	}
	else if (ft_strncmp(dir, "..", ft_strlen(dir)) == 0)
	{
		if (handle_parent_directory() == 1)
			return (1);
	}
	else
	{
		if (handle_regular_directory(dir, data) == 1)
			return (1);
	}
	return (0);
}
