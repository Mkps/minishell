/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 16:38:42 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		{
			exec_cmd(cmd, data);
			free_data(data);
			free(data->token_root);
			free(data->cmd_list);
			ft_free_tab(data->envv);
			exit (1);
		}
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