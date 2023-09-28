/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:26:03 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 07:09:29 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_shell(t_data *data)
{
	free_data(data);
	close_fd_set(data->old_fd[0], data->old_fd[1]);
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

void	free_child(t_data *data)
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
