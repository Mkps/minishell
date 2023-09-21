/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 16:59:48 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/21 12:10:40 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	init_data(t_data *data)
{
	ft_memset(data, '0', sizeof(data));
	data->token_root = (t_token **)ft_calloc(1, sizeof(t_token *));
	data->cmd_list = (t_cmd **)ft_calloc(1, sizeof(t_cmd *));
	*data->token_root = NULL;
	*data->cmd_list = NULL;
	data->parse_status = NONE;
	data->exit_status = g_exit_code;
	data->old_fd[0] = dup(STDIN_FILENO);
	data->old_fd[1] = dup(STDOUT_FILENO);
	data->env_cpy = NULL;
	data->export = NULL;
	data->user_input = NULL;
	data->flag = 0;
	return (EXIT_SUCCESS);
}
