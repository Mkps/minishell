/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:59:48 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 09:25:13 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	allocate_memory(t_data *data)
{
	data->token_root = (t_token **)ft_calloc(1, sizeof(t_token *));
	if (data->token_root == NULL)
		return (EXIT_FAILURE);
	*data->token_root = NULL;
	data->cmd_list = (t_cmd **)ft_calloc(1, sizeof(t_cmd *));
	if (!data->cmd_list)
		free_return (EXIT_FAILURE, data->token_root, NULL, NULL);
	*data->cmd_list = NULL;
	data->env_cpy = (t_env **)ft_calloc(1, sizeof(t_env *));
	if (data->env_cpy == NULL)
		free_return (EXIT_FAILURE, data->cmd_list, data->token_root, NULL);
	*data->env_cpy = NULL;
	data->env_export = (t_export **)ft_calloc(1, sizeof(t_export *));
	if (data->env_export == NULL)
		free_return (EXIT_FAILURE, data->cmd_list, data->token_root,
			data->env_cpy);
	*data->env_export = NULL;
	return (EXIT_SUCCESS);
}

int	init_data(t_data *data)
{
	ft_memset(data, '0', sizeof(data));
	if (allocate_memory(data) == EXIT_FAILURE)
	{
		output_err_cmd("Error while allocating memory", "init_data");
		return (2);
	}
	data->cmd_split = NULL;
	data->parse_status = NONE;
	data->exit_status = g_exit_code;
	data->old_fd[0] = dup(STDIN_FILENO);
	data->old_fd[1] = dup(STDOUT_FILENO);
	if (data->old_fd[0] < 0 || data->old_fd[1] < 0)
	{
		output_err_cmd("Failed to create original fd input", "init_data");
		return (EXIT_FAILURE);
	}
	data->user_input = NULL;
	data->raw_input = NULL;
	data->flag = 0;
	data->dir_flag = 0;
	return (EXIT_SUCCESS);
}
