/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:10:34 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/04 11:45:27 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	*dir;
	char	*pwd;
	char	*old_pwd;
	int		ret_value;

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
	old_pwd = NULL;
	ret_value = handle_directory_change(data, &old_pwd, dir);
	ret_value += ft_cd_next(pwd, dir, data, old_pwd);
	return (!(ret_value == 0));
}

int	ft_cd_next(char *pwd, char *dir, t_data *data, char *old_pwd)
{
	char	*temp;
	int		ret;

	ret = 0;
	(void)dir;
	if (pwd)
		free(pwd);
	pwd = getcwd(NULL, 0);
	temp = old_pwd;
	if (data->dir_flag == 0)
		ret = update_pwd_and_oldpwd(data, pwd, temp);
	free(pwd);
	free(old_pwd);
	data->dir_flag = 0;
	return (ret);
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

int	update_pwd_and_oldpwd(t_data *data, char *pwd, char *old_pwd)
{
	t_env		*current;
	size_t		env_count;
	size_t		i;
	int			ret_val;

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
			current->value = ft_strdup(old_pwd);
			break ;
		}
		current = current->next;
	}
	ret_val = cd_env_update(data, i, env_count);
	return (ret_val);
}
