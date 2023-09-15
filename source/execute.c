/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/15 13:47:11 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		execute_builtin(t_cmd *cmd, t_data *data)
{
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
	else if (ft_strncmp(cmd->cmd, ":", ft_strlen(cmd->cmd) + 1) == 0)
		ft_true();
	else if (ft_strncmp(cmd->cmd, "!", ft_strlen(cmd->cmd) + 1) == 0)
		ft_false();
    /*else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd + 1)) == 0)
        exit(0);*/
    else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
        ft_export(data);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
		ft_unset(data);
	return (0);
}

int		is_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strncmp(cmd->cmd, "echo", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "cd", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "pwd", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "env", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
		return (1);
	else if (ft_strncmp(cmd->cmd, ":", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	else if (ft_strncmp(cmd->cmd, "!", ft_strlen(cmd->cmd) + 1) == 0)
		return (0);
	return (0);
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{	
	/*if (cmd == NULL)
		printf("NULL\n");
	else
		printf("%s\n", cmd->cmd);*/
	if (cmd->type == EMPTY)
	{
		cmd->pid = fork();
		if (cmd->pid == 0)
		{		
			set_fd(cmd);
			cmd->pid = -2;
			cmd->pipe_status = 0;
			set_pipes(data, cmd);
			exit (1);
		}
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
			set_fd(cmd);
			set_pipes(data, cmd);
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
	t_cmd	*start;
	t_cmd	*last;
	int		i;

	status = 0;
	start = *data->cmd_list;
	if (!start)
		return ;
	i = 1;
	while(start) 
	{
		cmd = start;
		i = 1;
		while(i) 
		{
			i -= cmd->is_term;
			execute_cmd(cmd, data);
			cmd = cmd->next;
		}
		if (cmd == NULL)
			last = last_cmd(data->cmd_list);
		else
			last = cmd;
		int	wpid = 0;
		cmd = start;
		i = 1;
		while(i)
		{
			i -= cmd->is_term;
			close_pipes(data->cmd_list, NULL);
			wpid = waitpid(cmd->pid, &status, 0);
			if (wpid == last->pid)
				data->exit_status = WEXITSTATUS(status);
			cmd = cmd->next;
		}
		start = cmd;
	}
}
