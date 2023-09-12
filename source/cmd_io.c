#include "../include/minishell.h"

t_token *get_input_token(t_token *current_t)
{
	t_token	*tmp;
	t_token *input_token;

	input_token = NULL;
	tmp = current_t;
	while(tmp != NULL && tmp->token_type != PIPE)
	{
		if (tmp->token_type == IO_INPUT || tmp->token_type == IO_HEREDOC)
		{	
			input_token = tmp;
			return (input_token);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_token	*get_output_token(t_token *current)
{
	t_token *tmp;
	t_token *output_token;

	output_token = NULL;
	tmp = current;
	while (tmp != NULL && tmp->token_type != PIPE)
	{
		if (tmp != NULL && tmp->token_type != PIPE)
		{
			if (tmp->token_type == IO_TRUNC || tmp->token_type == IO_APPEND)
				output_token = tmp;
			tmp = tmp->next;
		}
	}
	return (output_token);
}

int		set_input_fd(t_token *current)
{
	int	token_type;

	token_type = current->token_type;
	if (token_is_quote(current->next))
		current = current->next;
	if (token_type == IO_INPUT)
	{
		char *str = "";
		if (token_is_quote(current) && current->next && token_is_quote(current->next))
			return (open_fd(0, str));
		return (open_fd(0, current->next->value));
	}
	if (token_type == IO_HEREDOC)
		here_doc_handler(current->next->value);
	return (0);
}
int		set_output_fd(t_token *current)
{
	int	token_type;

	token_type = current->token_type;
	if (current->next->token_type == DQUOTE)
		current = current->next;
	if (current->next->token_type != WORD)
		return (-1);
	if (token_type == IO_TRUNC)
		return (open_fd(1, current->next->value));
	else if (token_type == IO_APPEND)
		return (open_fd(2, current->next->value));
	return (-1);
}

void	handle_cmd_input(t_data *data, t_token *current_t, t_cmd *cmd)
{
	t_token	*input_token;
	int		p_type;

	cmd->fd[0] = 0;
	p_type = NONE;
	input_token = get_input_token(get_cmd_first(current_t));
	while (input_token != NULL)
	{
		cmd->fd[0] = set_input_fd(input_token);
		p_type = input_token->token_type;
		input_token = get_input_token(input_token->next);
		if (p_type == IO_HEREDOC && input_token && input_token->token_type == IO_HEREDOC)
			dup2(data->old_fd[0], 0);
		if (cmd->fd[0] < 0)
			return ;
	}
}
void	handle_cmd_output(t_data *data, t_token *current_t, t_cmd *cmd)
{
	t_token	*output_token;

	cmd->fd[1] = 1;
	output_token = get_output_token(get_cmd_first(current_t));
	while (output_token != NULL)
	{
		cmd->fd[1] = set_output_fd(output_token);
		output_token = get_output_token(output_token->next);	
		if (cmd->fd[1] < 0)
			return ;
	}
}

//	Looks for and then handles the io
void	handle_cmd_io(t_data *data, t_token *current_t, t_cmd *cmd)
{
	handle_cmd_input(data, current_t, cmd);

	handle_cmd_output(data, current_t, cmd);
}
