/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:29:45 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/26 15:47:17 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_pwd(char *pwd)
{
	if (pwd == NULL)
	{
		perror("getcwd");
		return (1);
	}
	return (0);
}

void	handle_parent_directory(void)
{
	if (chdir("..") != 0)
		perror("cd");
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
