/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 12:08:10 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/21 13:13:38 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Free shell
void	free_shell(t_data *data)
{
	free_data(data);
	free_env_lst(data->env_cpy);
	free(data->token_root);
	free(data->cmd_list);
	if (data->cmd_split)
		ft_free_tab(data->cmd_split);
	ft_free_tab(data->envv);
}
void	free_subshell(t_data *data)
{

}

void	free_child(t_data *data)
{
	free_data(data);
	free(data->token_root);
	free(data->cmd_list);
	ft_free_tab(data->cmd_split);
	ft_free_tab(data->envv);
}

// void	data_cleanup(t_data *data)
// {
// 	free_data(data);
// 	free_env_lst(data->env_cpy);
// 	free(data->token_root);
// 	free(data->cmd_list);
// 	ft_free_tab(data->envv);
// }
