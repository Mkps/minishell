/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:29:45 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/28 18:29:11 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <string.h>

int	set_pwd(char *pwd)
{
	if (pwd == NULL)
	{
		output_err_cmd(strerror(errno), "getcwd");
		return (1);
	}
	return (0);
}

void	handle_parent_directory(void)
{
	if (chdir("..") != 0)
		output_err_cmd(strerror(errno), "cd");
}

void	handle_previous_directory(t_data *data, char **old_pwd)
{
	char	*old_pwd_env;

	old_pwd_env = ft_getenv(data->envv, "OLDPWD");
	if (old_pwd_env == NULL)
	{
		output_err_cmd("OLDPWD not set", "cd");
		return ;
	}
	*old_pwd = ft_strdup(old_pwd_env);
	if (chdir(old_pwd_env) != 0)
	{
		output_err_cmd(strerror(errno), "cd");
		free(*old_pwd);
	}
}

void	ft_cd_next(char *pwd, char *tmp, t_data *data, char *old_pwd)
{
	char	*temp;

	if (pwd)
		free(pwd);
	pwd = getcwd(NULL, 0);
	temp = pwd;
	update_pwd_and_oldpwd(data, pwd, temp);
	ft_setenv(data, tmp);
	free(pwd);
	free(old_pwd);
}
