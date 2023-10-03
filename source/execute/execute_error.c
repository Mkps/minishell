/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:21:53 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:27 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cmd_is_dir(t_cmd *cmd)
{
	struct stat	stat_var;

	ft_memset(&stat_var, 0, sizeof(stat_var));
	stat(cmd->cmd, &stat_var);
	return (S_ISDIR(stat_var.st_mode));
}

int	is_cmd_fko(t_cmd *cmd, t_data *data)
{
	char	**env_p;
	char	*tmp;
	int		ret;

	env_p = get_path(data->envv);
	tmp = get_cmd(cmd->cmd, env_p);
	ret = 0;
	if (tmp == NULL)
		ret = 1;
	else
	{
		if (access(tmp, F_OK | X_OK))
			ret = 2;
	}
	if (env_p)
		ft_free_tab(env_p);
	if (tmp)
		free(tmp);
	return (ret);
}

int	get_cmd_ecode(t_cmd *cmd, t_data *data)
{
	if (is_cmd_fko(cmd, data) == 1 || (cmd->type == WORD && cmd->cmd[0] == 0))
	{
		output_err_cmd("command not found", cmd->cmd);
		return (CMD_ERR_FKO);
	}
	if (is_cmd_fko(cmd, data) == 2)
	{
		output_err_cmd(strerror(errno), cmd->cmd);
		return (CMD_ERR_XKO);
	}
	if (cmd_is_dir(cmd))
	{
		output_err_cmd("Is a directory", cmd->cmd);
		return (CMD_ERR_XKO);
	}
	return (EXIT_FAILURE);
}
