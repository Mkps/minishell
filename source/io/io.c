/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:22:28 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 15:35:10 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_io_node	*create_io_node(char *filename, int mode)
{
	t_io_node	*ret;

	ret = malloc(sizeof(t_io_node));
	ret->fd = -2;
	ret->filename = filename;
	ret->mode = mode;
	ret->next = NULL;
	return (ret);
}

int	add_io_node(t_data *data, t_cmd *cmd, char *filename, int mode)
{
	t_io_node	*current;

	(void)data;
	if (*cmd->io_list == NULL)
	{
		current = create_io_node(filename, mode);
		*cmd->io_list = current;
		if (!current)
			return (0);
		return (1);
	}
	current = *cmd->io_list;
	while (current->next)
		current = current->next;
	current->next = create_io_node(filename, mode);
	if (!current)
		return (0);
	return (1);
}

t_token	*get_io_token(t_token *current_t)
{
	t_token	*tmp;
	t_token	*io_token;

	io_token = NULL;
	tmp = current_t;
	while (tmp != NULL && !token_is_term(tmp))
	{
		if (token_is_io(tmp))
		{
			io_token = tmp;
			return (io_token);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_filename(t_token *io_token)
{
	t_token	*current;

	current = io_token;
	if (current->token_type == IO_HEREDOC)
	{
		if (current->next && current->next->token_type == WORD
			&& current->next->quote_status == IO_HEREDOC)
			return (ft_strdup(current->next->value));
		else if (current->next && current->next->token_type == WORD
			&& current->next->quote_status == SQUOTE)
			return (ft_strappend("'", current->next->value, 0));
	}
	if (current->next && current->next->token_type == WORD)
	{
		if (current->next->quote_status == 0 && current->next->value[0] == 0)
			return (chrtostr(4));
		else
			return (current->next->value);
	}
	else if (current->next && token_is_quote(current->next)
		&& current->next->next && current->next->next->token_type == WORD)
		return (current->next->next->value);
	return (NULL);
}

//	Looks for and then handles the io
int	handle_cmd_io(t_data *data, t_token *current_t, t_cmd *cmd)
{
	t_token	*io_token;

	io_token = get_io_token(get_cmd_first(current_t));
	while (io_token != NULL)
	{
		if (!add_io_node(data, cmd, get_filename(io_token),
				io_token->token_type))
			return (EXIT_FAILURE);
		io_token = get_io_token(io_token->next);
	}
	return (EXIT_SUCCESS);
}
