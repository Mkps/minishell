/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:41:18 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:45:56 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	go_next(t_token *current_t)
{
	if (current_t
		&& (!current_t->prev
			|| token_is_term(current_t->prev))
		&& (is_assign(current_t->value)
			&& current_t->quote_status != O_PAR))
	{
		return (1);
	}
	return (0);
}

t_token	*handle_cmd(t_data *data, t_token *tmp, t_token *current_t)
{
	tmp = get_next_cmd(tmp);
	current_t = tmp;
	if (tmp && tmp->token_type != WORD)
		add_empty_cmd(data);
	else
		current_t = add_cmd(data, current_t);
	handle_cmd_io(data, current_t, last_cmd(data->cmd_list));
	handle_assign(data, current_t, last_cmd(data->cmd_list));
	while (current_t && !token_is_term(current_t))
		current_t = current_t->next;
	if (current_t && current_t->token_type == PIPE)
		set_pipe(last_cmd(data->cmd_list));
	if (current_t && current_t->token_type >= TERM_END
		&& current_t->token_type <= TERM_OR)
		last_cmd(data->cmd_list)->is_term = current_t->token_type;
	return (current_t);
}

void	build_cmd_list(t_data *data, t_token *token)
{
	t_token	*current_t;
	t_token	*tmp;

	if (token == NULL)
		current_t = *data->token_root;
	else
		current_t = token;
	while (current_t != NULL)
	{
		tmp = current_t;
		if (go_next(current_t))
			current_t = current_t->next;
		else if (current_t)
		{
			current_t = handle_cmd(data, tmp, current_t);
			current_t = current_t->next;
		}
		else
		{
			if (current_t)
				current_t = current_t->next;
		}
	}
}
