/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:31:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/21 15:11:10 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>
#include <unistd.h>

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
    else if (ft_strncmp(cmd->cmd, "export", ft_strlen(cmd->cmd) + 1) == 0)
    {
	    ft_export(data);
		return (1);
	}
	else if (ft_strncmp(cmd->cmd, "unset", ft_strlen(cmd->cmd) + 1) == 0)
	{
		ft_unset(data);
		return (1);
	}
	/*else if (ft_strncmp(cmd->cmd, "exit", ft_strlen(cmd + 1)) == 0)
        exit(0);*/
	else if (ft_strncmp(cmd->cmd, ":", ft_strlen(cmd->cmd) + 1) == 0)
		ft_true();
	else if (ft_strncmp(cmd->cmd, "!", ft_strlen(cmd->cmd) + 1) == 0)
		ft_false();	
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

int	cmd_is_dir(t_cmd *cmd, t_data *data)
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
	if (cmd_is_dir(cmd, data))
	{
		output_err_cmd("Is a directory", cmd->cmd);
		return (CMD_ERR_XKO);
	}
	return (EXIT_FAILURE);
}
void	set_var_cmd(t_data *data, t_cmd *cmd)
{
	t_env	*start;

	start = *cmd->assign;
	if (!start)
		return ;
	while (start)
	{
		ft_setenv(data, start->value);
		start = start->next;
	}
}
void	execute_cmd(t_cmd *cmd, t_data *data)
{
	int	exit_code;

	if (cmd->type == EMPTY)
	{
		set_var_cmd(data, cmd);
		cmd->pid = fork();
		if (cmd->pid == 0)
		{		
			cmd->pipe_status = 0;
			set_pipes(data, cmd);
			set_fd(data, cmd);
			exit (0);
		}
		return ;
	}
	free_var(data, cmd);
	if (cmd->type == O_PAR)
	{
		cmd->pid = fork();
		if (cmd->pid == 0)
		{		
			set_pipes(data, cmd);
			set_fd(data, cmd);
			close_pipes(data->cmd_list, NULL, NULL);
			minishell_subshell(data, cmd->cmd);
		}
	}
	else
	{
		if (is_builtin(cmd, data) == 1)
		{
			set_fd(data, cmd);
			cmd->pid = -2;
			execute_builtin(cmd, data);
		}
		else
		{
			cmd->pid = fork();
			if (cmd->pid == 0)
			{
				close(data->old_fd[0]);
				close(data->old_fd[1]);
				set_pipes(data, cmd);		
				if (cmd->fd[0] > -1)
				{
					dup2(cmd->fd[0], STDIN_FILENO);
					close(cmd->fd[0]);
				}
				if (cmd->fd[1] > -1)
				{
					dup2(cmd->fd[1], STDOUT_FILENO);
					close(cmd->fd[1]);
				}
				close_pipes(data->cmd_list, NULL, NULL);
				if (!execute_builtin(cmd,data))
					exec_cmd(cmd, data);
				exit_code = get_cmd_ecode(cmd, data);
				free_data(data);
				free(data->token_root);
				free(data->cmd_list);
				ft_free_tab(data->cmd_split);
				ft_free_tab(data->envv);
				exit (exit_code);
			}
		}
	}
}

void	execute(t_data *data)	
{
	int		status;
	int		eval;
	t_cmd	*cmd;
	t_cmd	*start;
	t_cmd	*last;
	int		i;
	int		wpid;

	status = 0;
	g_exit_code = 0;
	eval = 0;
	start = *data->cmd_list;
	if (!start)
		return ;
	i = 1;
	while(start) 
	{
		cmd = start;
		i = 1;
		while(i > 0 && cmd) 
		{
			i -= cmd->is_term;
			execute_cmd(cmd, data);
			cmd = cmd->next;
		}
		if (cmd == NULL)
			last = last_cmd(data->cmd_list);
		else
			last = cmd;
		wpid = 0;
		cmd = start;
		i = 1;
		while(i > 0 && cmd)
		{
			i -= cmd->is_term;
			if (cmd->fd[0] > -1)
				close(cmd->fd[0]);
			if (cmd->fd[1] > -1)
				close(cmd->fd[1]);
			close_pipes(&start, NULL, last);
			if (cmd->pid > 0)
				wpid = waitpid(cmd->pid, &status, 0);
			if (cmd->is_term != 0)
			{
				if (!g_exit_code)
					g_exit_code = WEXITSTATUS(status);
				eval = g_exit_code;
			}
			cmd = cmd->next;
		}
		if (g_exit_code > 127)
			break ;
		while ((cmd && eval == 0 && cmd->prev->is_term == TERM_OR) || (cmd && eval > 0 && cmd->prev->is_term == TERM_2AND))
		{
			if (cmd->is_term)
				cmd = cmd->next;
			else
			{
				while (!cmd->is_term)
					cmd = cmd->next;
				cmd = cmd->next;
			}
		}
		start = cmd;
	}
}
