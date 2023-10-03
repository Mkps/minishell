/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_chdir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:17:41 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 15:02:39 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_regular_directory(char *dir)
{
//	if (access(dir, F_OK) == -1)
//		printf("minishell: cd: %s: No such file or directory\n", dir);
	//else 
	if (chdir(dir) != 0)
		output_err_cmd(strerror(errno), "cd");
}

void	handle_home_directory(t_data *data, const char *dir)
{
	char	*home_dir;
	size_t	full_path_len;
	char	*full_path;

	home_dir = ft_getenv(data->envv, "HOME");
	if (home_dir == NULL)
	{
		output_err_cmd(strerror(errno), "cd");
		printf("minishell: cd: HOME not set\n");
		return ;
	}
	full_path_len = strlen(home_dir) + strlen(dir) - 1;
	full_path = (char *)malloc(full_path_len + 1);
	if (full_path == NULL)
	{
		output_err_cmd(strerror(errno), "cd: malloc:");
		return ;
	}
	ft_strlcpy(full_path, home_dir, full_path_len + 1);
	ft_strlcat(full_path, dir + 1, full_path_len + 1);
	if (chdir(full_path) != 0)
		output_err_cmd(strerror(errno), "cd");
	free(full_path);
}

void	handle_previous_directory(t_data *data, char **old_pwd)
{
	char	*old_pwd_env;

	(void)old_pwd;
	old_pwd_env = ft_getenvcpy(data, "OLDPWD");
	if (old_pwd_env == NULL)
	{
		output_err_cmd("OLDPWD not set", "cd");
		return ;
	}
	printf("old pwd = %s\n", old_pwd_env);
	if (chdir(old_pwd_env) != 0)
		output_err_cmd(strerror(errno), "cd");
}

void	handle_parent_directory(void)
{
	if (chdir("..") != 0)
		output_err_cmd(strerror(errno), "cd");
}

void	handle_directory_change(t_data *data, char **old_pwd, char *dir)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (*old_pwd)
		free(*old_pwd);
	*old_pwd = current_dir;
	if (ft_strncmp(dir, "~", ft_strlen(dir)) == 0)
		handle_home_directory(data, dir);
	else if (ft_strncmp(dir, "..", ft_strlen(dir)) == 0)
		handle_parent_directory();
	else
		handle_regular_directory(dir);
}
