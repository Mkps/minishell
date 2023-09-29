/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 12:08:10 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 17:21:21 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	free_data(t_data *data)
{
	free_cmd_list(data);
	free_token(data);
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

	(void)data;
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
