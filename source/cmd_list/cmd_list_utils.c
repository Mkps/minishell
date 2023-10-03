/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 11:41:38 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 16:16:07 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Checks if the str(cmd->cmd or token->value) represents an assign command.
int	is_assign(char *str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while (*str)
	{
		if (*str == '=' && *str + 1)
			return (1);
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (0);
}

int	is_empty_cmd(t_token *start)
{
	t_token	*tmp;

	tmp = start;
	while (tmp && !token_is_term(tmp))
	{
		if (token_is_io(tmp))
			return (1);
		tmp = tmp->next;
	}
	while (tmp && tmp != start)
	{
		tmp = tmp->prev;
	}
	return (0);
}

// Sets up the pipe and sets pipe_status to 1.
int	set_pipe(t_cmd *cmd)
{
	cmd->pipe_status = 1;
	cmd->pipe_fd = (int *)malloc(sizeof(int *) * 2);
	if (cmd->pipe_fd && pipe(cmd->pipe_fd) != -1)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

//	Returns the type of the cmd. If not a special case it is handled as a CMD 
int	get_cmd_type(t_token *token)
{
	if (!token->value)
		return (EMPTY);
	if (token->value[0] == '#')
		return (COMMENT);
	if (token->quote_status == O_PAR)
		return (O_PAR);
	return (CMD);
}
