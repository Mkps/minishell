/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:10:34 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/29 16:38:16 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	*dir;
	char	*pwd;
	char	*tmp;
	char	*old_pwd;

	tmp = NULL;
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
	if (set_pwd(pwd, dir) == 1)
		return (free(pwd), EXIT_FAILURE);
	else if (pwd)
		tmp = ft_strjoin("PWD=", pwd);
	old_pwd = NULL;
	handle_directory_change(data, &old_pwd, dir);
	ft_cd_next(pwd, tmp, data, old_pwd);
	return (EXIT_SUCCESS);
}

void	ft_cd_next(char *pwd, char *tmp, t_data *data, char *old_pwd)
{
	char	*temp;

	if (pwd)
		free(pwd);
	pwd = getcwd(NULL, 0);
	temp = old_pwd;
	update_pwd_and_oldpwd(data, pwd, temp);
	if (tmp)
		ft_setenv(data, tmp);
	free(pwd);
	free(old_pwd);
}

void	update_pwd(t_data *data, char *pwd)
{
	t_env	*current;

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
	t_env	*current;

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
			current->value = ft_strdup(old_pwd);
			break ;
		}
		current = current->next;
	}
}
