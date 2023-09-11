/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/11 14:13:14 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		execute_builtin(t_cmd *cmd, t_data *data)
{
	return (0);
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	if (cmd->type == EMPTY)
	{
		free_data(data);
		free(data->token_root);
		free(data->cmd_list);
		ft_free_tab(data->envv);
		exit (1);
	}
	if (execute_builtin(cmd, data))
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