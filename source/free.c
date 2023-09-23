/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 12:08:10 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 01:07:16 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Free shell
void	free_shell(t_data *data)
{
	free_data(data);
	if (data->token_root)
		free(data->token_root);
	if (data->cmd_list)
		free(data->cmd_list);
	if (data->env_cpy)
	{
		free_env_list(data->env_cpy);
		free(data->env_cpy);
	}
	if (data->env_export)
	{
		free_export_list(data->env_export);
		free(data->env_export);
	}
	if (data->cmd_split)
		ft_free_tab(data->cmd_split);
	if (data->envv)
		ft_free_tab(data->envv);
}

void	free_cmd_list(t_data *data)
{
	t_cmd		*current;
	t_cmd		*tmp;
	t_env		*env;
	t_env		*next;
	t_io_node	*io;
	t_io_node	*next_io;

	if (*data->cmd_list == NULL)
		return ;
	current = *data->cmd_list;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		if (tmp->type != EMPTY && tmp->args)
			ft_free_tab(tmp->args);
		if (tmp->type != EMPTY && tmp->cmd)
			free(tmp->cmd);
		if (tmp->pipe_status)
			free(tmp->pipe_fd);
		if (tmp->assign)
		{
			env = *tmp->assign;
			while (env)
			{
				next = env->next;
				free(env->key);
				free(env);
				env = next;
			}
			free(tmp->assign);
		}
		if (tmp->io_list)
		{
			io = *tmp->io_list;
			while (io)
			{
				next_io = io->next;
				if (io->fd > -1)
					close(io->fd);
				free(io);
				io = next_io;
			}
			free(tmp->io_list);
			dup2(data->old_fd[0], STDIN_FILENO);
		}
		free(tmp);
	}
	*data->cmd_list = NULL;
}
void	free_subshell(t_data *data)
{
}

int	free_return(int return_value, void *ptr_1, void *ptr_2, void *ptr_3)
{
	if (ptr_1 != NULL)
		free(ptr_1);
	if (ptr_2 != NULL)
		free(ptr_2);
	if (ptr_3 != NULL)
		free(ptr_3);
	return (return_value);
}

void	multi_free(void *ptr_1, void *ptr_2, void *ptr_3, void *ptr_4)
{
	if (ptr_1 != NULL)
		free(ptr_1);
	if (ptr_2 != NULL)
		free(ptr_2);
	if (ptr_3 != NULL)
		free(ptr_3);
	if (ptr_4 != NULL)
		free(ptr_4);
}
void	free_child(t_data *data)
{
	free_data(data);
	free(data->token_root);
	free(data->cmd_list);
	if (data->env_cpy)
		free_env_list(data->env_cpy);
	if (data->env_export)
		free_export_list(data->env_export);
	free(data->env_cpy);
	free(data->env_export);
	ft_free_tab(data->cmd_split);
	ft_free_tab(data->envv);
}

void	free_heredoc_child(t_data *data)
{
	free_data(data);
	free(data->token_root);
	free(data->cmd_list);
	if (data->env_cpy)
		free_env_list(data->env_cpy);
	if (data->env_export)
		free_export_list(data->env_export);
	free(data->env_cpy);
	free(data->env_export);
	ft_free_tab(data->cmd_split);
}

int	free_data(t_data *data)
{
	free_token(data);
	free_cmd_list(data);
	if (data->user_input)
	{
		free(data->user_input);
		data->user_input = NULL;
	}
	if (data->raw_input)
	{
		free(data->raw_input);
		data->raw_input = NULL;
	}
	return (EXIT_SUCCESS);
}

void	free_token(t_data *data)
{
	t_token	*current;
	t_token	*tmp;

	if (*data->token_root == NULL)
		return ;
	current = *data->token_root;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
	*data->token_root = NULL;
}
void	free_var(t_data *data, t_cmd *cmd)
{
	t_env	*current;
	t_env	*next;

	if (cmd->assign)
	{
		current = *cmd->assign;
		while (current)
		{
			free(current->value);
			current = current->next;
		}
	}
}
