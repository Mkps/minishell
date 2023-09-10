#include "../include/minishell.h"

char	*ft_strappend(char *s1, char *s2, int mode)
{
	char	*tmp;
	int		tmp_len;
	int		i;
	int		j;

	tmp_len = ft_strlen(s1) + ft_strlen(s2);
	tmp = (char *)malloc(sizeof(char) * (tmp_len + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while(s1[i])
	{
		tmp[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		tmp[i + j] = s2[j];
		j++;
	}
	tmp[i + j] = 0;
	if (mode == 1)
		free(s2);
	if (mode == 2)
		free(s1);
	if (mode == 3)
	{
		free(s1);
		free(s2);
	}
	return (tmp);
}

//	Parses tokens looking for VAR to expand.
void	parse_token(t_data *data)
{
	t_token	*current;

	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD & current->quote_status != OSQUOTE)
		{
			current->value = var_expander(data, current->value);	
		}
		current = current->next;
	}
}

// Creating a cmd.
t_cmd	*create_cmd(t_data *data)
{
	t_cmd	*ret;

	ret = (t_cmd *)malloc(sizeof(t_cmd) * 1);
	if (!ret)
		return (NULL); 
	ret->next = NULL;
	ret->prev = NULL;
	ret->pipe_status = 0;
	return (ret);
}

//	Adds a cmd to the back of the cmd list.
void	add_cmd_back(t_data *data)
{
	t_cmd	*last;

	if (*data->cmd_list == NULL)
	{
		*data->cmd_list = create_cmd(data);
		return ;
	}
	last = *data->cmd_list;
	while (last->next != NULL)
		last = last->next;
	last->next = create_cmd(data);
	last_cmd(data->cmd_list)->prev = last;
}

// Checks if the str(cmd->cmd or token->value) represents an assign command.
int	is_assign(char	*str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while(*str)
	{
		if (*str == '=' && *str + 1)
			return (1); 
		str++;
	}
	return (0);
}

// Returns the last cmd of the cmd_list
t_cmd	*last_cmd(t_cmd **root)
{
	t_cmd	*current;

	current = *root;
	if (!current)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}

//	Checks if the cmd is a comment. NOTE: will be reworked
int		get_cmd_type(char *value)
{
	if (!value)
		return (EMPTY);
	if (*value == '#')
		return (COMMENT);
	return (CMD);
}

t_token	*add_cmd(t_data *data, t_token *token)
{
	t_cmd	*new_cmd;
	t_token	*current;
	int		type;
	char	*tmp;
	char	*new_tmp;
	char	**args;

	add_cmd_back(data);
	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = token->value; 
	new_cmd->type = get_cmd_type(token->value);
	new_cmd->fd[0] = -1;
	new_cmd->fd[1] = -1;
	tmp = "";
	tmp = ft_strappend(tmp, ";", 0);
	current = token;
	while (current->token_type == WORD )
	{
		tmp = ft_strappend(tmp, ";", 2);	
		tmp = ft_strappend(tmp, current->value, 2);	
		if (current->next->token_type == OSQUOTE || current->next->token_type == ODQUOTE)
		{
			type = current->next->token_type;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	new_cmd->args = ft_split(tmp, ';');
	free(tmp);
	return (current);
}

void	add_assign_cmd(t_data *data)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd(data);
	new_cmd->type = CMD_ASSIGN;
	new_cmd->fd[0] = -1;
	new_cmd->fd[1] = -1;
}
t_token	*get_cmd_first(t_token *current_t)
{
	t_token	*current;

	current = current_t;
	while(current != NULL && current->prev != NULL && current->prev->token_type != PIPE)
		current = current->prev;
	return (current);
}

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
	if (current->next->token_type != WORD)
		return (-1);
	if (token_type == IO_INPUT)
		return (open_fd(0, current->next->value));
	if (token_type == IO_HEREDOC)
		here_doc_handler(current->next->value);
	return (0);
}
int		set_output_fd(t_token *current)
{
	int	token_type;

	token_type = current->token_type;
	if (current->next->token_type == ODQUOTE)
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

// Returns the first encountered cmd token
t_token	*get_next_cmd(t_token *src)
{
	t_token	*current;

	current = src;
	while (current != NULL && !token_is_term(current))
	{
		if (current != NULL && current->token_type == WORD && !is_assign(current->value))
		{
			if (current->prev == NULL || current->quote_status == NONE && current->prev != NULL && !token_is_io(current->prev) )
			{
				return (current);
			}
			if (current->prev != NULL && current->quote_status != NONE && !token_is_io(current->prev->prev))
				return (current);
		}
		current = current->next;
	}
	return (NULL);
}

// Sets up the pipe and sets pipe_status to 1.
void	set_pipe(t_cmd *cmd)
{
	cmd->pipe_status = 1;
	cmd->pipe_fd = (int *)malloc(sizeof(int *) * 2);
	pipe(cmd->pipe_fd);
	return ;	
}

void	build_cmd_list(t_data *data, t_token *token)
{
	t_token	*current_t;

	if (token == NULL)
		current_t  = *data->token_root;
	else
		current_t = token;
	while (current_t != NULL)
	{
		if (current_t && !current_t->prev && is_assign(current_t->value))
		{
			ft_setenv(data, current_t->value);
			current_t = current_t->next;
		}
		if (current_t && (current_t = get_next_cmd(current_t)) != NULL)
		{
			current_t = add_cmd(data, current_t);
			handle_cmd_io(data, current_t, last_cmd(data->cmd_list));
			while (!token_is_term(current_t))
				current_t = current_t->next;
			if (current_t && current_t->token_type == PIPE)
				set_pipe(last_cmd(data->cmd_list));
			current_t = current_t->next;
		}
		else if (current_t)
		 	current_t = current_t->next;
	}

}
