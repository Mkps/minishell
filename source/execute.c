/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/11 16:17:53 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		execute_builtin(t_cmd *cmd, t_data *data)
{
	//printf("builtin\n");
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd) + 1) == 0)
	{
	    ft_echo(cmd);
		return (1);
	}
	else if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd) + 1) == 0)
    {
		ft_cd(cmd, data);
		return (1);
	}
	else if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd) + 1) == 0)
	{
		ft_pwd(data);
		return (1);
	}
	/*else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
        ft_env(data);
    else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd + 1)) == 0)
        exit(0);
    else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd + 1)) == 0)
        ft_export(cmd, data);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd + 1)) == 0)
		ft_unset(cmd, data);*/
	return (0);
}

int		is_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	// else if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd) + 1) == 0)
	// 	return (1);
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
    else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
    else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	return (0);
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	if (cmd->type == EMPTY)
	{
		set_fd(cmd);
		return ;
	}
	if (is_builtin(cmd, data) == 1)
	{
		set_fd(cmd);
		execute_builtin(cmd, data);
	}
	else
	{
		cmd->pid = fork();
		if (cmd->pid == 0)
		{
			set_pipes(data, cmd);
			set_fd(cmd);
			close_pipes(data->cmd_list, NULL);
			if (!execute_builtin(cmd,data))
				exec_cmd(cmd, data);
			free_data(data);
			free(data->token_root);
			free(data->cmd_list);
			ft_free_tab(data->envv);
			exit (1);
		}
	}
}

void	execute(t_data *data)	
{
	int		status;
	t_cmd	*cmd;

	status = 0;
	cmd = *data->cmd_list;
	if (!cmd)
		return ;
	while(cmd) 
	{
		execute_cmd(cmd, data);
		cmd = cmd->next;
	}
	int	wpid = 0;
	cmd = *data->cmd_list;
	while(cmd) 
	{
		close_pipes(data->cmd_list, NULL);
		wpid = waitpid(cmd->pid, &status, 0);
		if (wpid == last_cmd(data->cmd_list)->pid)
			data->exit_status = status;
		cmd = cmd->next;
	}
}
