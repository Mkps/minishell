/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 12:29:45 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/29 16:44:36 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	*ft_getenvcpy(t_data *data, char *key)
{
	t_env	*current;

	current = *data->env_cpy;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	ft_cd_next(char *pwd, char *tmp, t_data *data, char *old_pwd)
{
	char	*temp;

	if (pwd)
		free(pwd);
	pwd = getcwd(NULL, 0);
	temp = old_pwd;
	update_pwd_and_oldpwd(data, pwd, temp);
	ft_setenv(data, tmp);
	free(pwd);
	free(old_pwd);
}
