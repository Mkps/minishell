/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/12 12:55:00 by uaupetit         ###   ########.fr       */
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
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
    {
	    ft_env(data);
		return (1);
	}
	else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
    {
	    ft_export(data);
		return (1);
	}
	/*else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd + 1)) == 0)
        exit(0);
    else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd + 1)) == 0)
        ft_export(cmd, data);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd + 1)) == 0)
		ft_unset(cmd, data);*/
	return (0);
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	if (execute_builtin(cmd, data) == 1)
	{
		free_data(data);
		free(data->token_root);
		free(data->cmd_list);
		ft_free_tab(data->envv);
		exit (1);
	}
	else
	{
		exec_cmd(cmd, data);
		free_data(data);
		free(data->token_root);
		free(data->cmd_list);
		ft_free_tab(data->envv);
		exit (1);
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
		cmd->pid = fork();
		if (cmd->pid == 0)
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

void 	ft_env(t_data *data)
{
    t_env *current = data->env_cpy;

    while (current != NULL)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}
