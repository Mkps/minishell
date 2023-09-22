/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:59:48 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/21 22:49:52 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_data(t_data *data)
{
	ft_memset(data, '0', sizeof(data));
	data->token_root = (t_token **)ft_calloc(1, sizeof(t_token *));
	*data->token_root = NULL;
	data->cmd_list = (t_cmd **)ft_calloc(1, sizeof(t_cmd *));
	*data->cmd_list = NULL;
	data->env_cpy = (t_env **)ft_calloc(1, sizeof(t_env *));
	*data->env_cpy = NULL;
	data->env_export = (t_export **)ft_calloc(1, sizeof(t_export *));
	*data->env_export = NULL;
	data->parse_status = NONE;
	data->exit_status = g_exit_code;
	data->old_fd[0] = dup(STDIN_FILENO);
	data->old_fd[1] = dup(STDOUT_FILENO);
	data->user_input = NULL;
	data->raw_input = NULL;
	data->flag = 0;
	return (EXIT_SUCCESS);
}
