#include "../include/minishell.h"
#include <stdlib.h>

int		is_assign(char *str);

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
	while (s1[i])
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

int	token_wc(char *input, t_token *current, t_data *data)
{
	int	i;
	int	current_status;

	i = 0;
	if (!input)
	{
		return (1);
	}
	while (*(input + i) && ft_get_sep_type(input + i) != WSPACE)
		i++;
	if (i > 0)
		insert_token_next(current, WORD, ft_str_extract(input, i));
	while (*(input + i) != 0 && ft_get_sep_type(input + i) == WSPACE)
		i++;
	if (i == 0)
		i++;
	return (i);
}

t_token	*wc_tokenize(t_token *start, char *str, t_data *data)
{
	char	*tmp;
	int		i;
	t_token	*node;
	t_token	*ret;
	t_token	*swap;
	t_token	*next;

	i = 0;
	tmp = ft_wildcard(start->value);
	node = start;
	while (tmp[i])
	{
		i += token_wc(tmp + i, node, data);
		node = node->next;
	}
	ret = node;
	if (!ret)
		return (start);
	swap = start->prev;
	next = start->next;
	if (next)
	{
		swap->next = next;
		next->prev = swap;
		free(start);
		start = NULL;
	}
	free(tmp);
	return (ret);
}

//	Parses tokens looking for things to expand.
void	parse_token(t_data *data)
{
	t_token	*current;
	t_token	*node;
	char	*tmp;

	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD && current->quote_status != SQUOTE
			&& current->quote_status != O_PAR
			&& current->quote_status != IO_HEREDOC)
		{
			var_expander(data, current->value, current);
			if (current->quote_status == NONE  && current->next && (!current->value || current->value && current->value[0] == 0))
			{
				current = current->next;
				lst_del_prev(&current);
				if (current->prev == NULL)
					*data->token_root = current;
			}
			else if (current->quote_status == NONE && current->prev && (!current->value || current->value && current->value[0] == 0))
			{
				current = current->prev;
				lst_del_next(&current);
			}
			else if (current->quote_status == NONE && (!current->value || current->value && current->value[0] == 0))
			{
				lst_del_token(&current);
				*data->token_root = NULL;
			}
		}
		if (current->token_type == WORD && current->quote_status == NONE)
		{
			if (current->prev && current->prev->token_type == WORD
				&& !is_assign(current->prev->value)
				&& wc_present(current->value))
			{
				current = wc_tokenize(current, current->value, data);
			}
		}
		if (current->token_type == IO_HEREDOC && current->next
			&& ((current->next->token_type == WORD)
				|| token_is_quote(current->next)
				&& current->next->next->token_type == WORD))
		{
			if (current->next && (current->next->token_type == WORD))
				current->next->quote_status = DQUOTE;
			else if (current->next && ((current->next->token_type == WORD)
						|| token_is_quote(current->next)
						&& current->next->next->token_type == WORD))
				current->next->next->quote_status = SQUOTE;
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
	ret->assign = (t_env **)malloc(sizeof(t_env *) * 1);
	ret->io_list = (t_io_node **)malloc(sizeof(t_io_node *));
	if (!ret->assign || !ret->io_list)
	{
		free(ret);
		if (ret->assign)
			free(ret->assign);
		else if (ret->io_list)
			free(ret->io_list);
		return (NULL);
	}
	ret->next = NULL;
	ret->prev = NULL;
	*ret->io_list = NULL;
	*ret->assign = NULL;
	ret->pipe_status = 0;
	ret->is_bg = 0;
	ret->is_term = 0;
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

t_token	*add_cmd(t_data *data, t_token *token)
{
	t_cmd	*new_cmd;
	t_token	*current;
	int		type;
	char	*tmp;
	char	*new_tmp;
	char	**args;
	int		i;
	char	sep[2];
		char test[2];

	add_cmd_back(data);
	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = ft_strdup(token->value);
	new_cmd->type = get_cmd_type(token);
	new_cmd->fd[0] = -2;
	new_cmd->fd[1] = -2;
	tmp = "";
	current = token;
	sep[0] = 2;
	sep[1] = 0;
	tmp = ft_strappend(tmp, sep, 0);
	while (current->token_type == WORD)
	{
		tmp = ft_strappend(tmp, sep, 2);
		test[0] = 1;
		test[1] = 0;
		if (current->value[0] == 0)
			tmp = ft_strappend(tmp, test, 2);
		else
			tmp = ft_strappend(tmp, current->value, 2);
		if (current->next->token_type == SQUOTE
			|| current->next->token_type == DQUOTE)
		{
			type = current->next->token_type;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	new_cmd->args = ft_split(tmp, 2);
	i = -1;
	while (new_cmd->args[++i])
	{
		if (new_cmd->args[i][0] == 1)
			new_cmd->args[i][0] = '\0';
	}
	// if (new_cmd->type == O_PAR)
	// 	new_cmd->is_term = O_PAR;
	free(tmp);
	return (current);
}

void	add_empty_cmd(t_data *data)
{
	t_cmd	*new_cmd;

	add_cmd_back(data);
	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = NULL;
	new_cmd->type = EMPTY;
	new_cmd->fd[0] = -1;
	new_cmd->fd[1] = -1;
}

t_token	*get_cmd_first(t_token *current_t)
{
	t_token	*current;

	current = current_t;
	if (current->prev == NULL)
		return (current);
	while (current != NULL && current->prev != NULL
		&& !token_is_term(current->prev))
		current = current->prev;
	return (current);
}

// Returns the first encountered cmd token
t_token	*get_next_cmd(t_token *src)
{
	t_token	*current;

	current = src;
	while (current != NULL && !token_is_term(current))
	{
		if (current != NULL && current->token_type == WORD
			&& (!is_assign(current->value) || current->quote_status == O_PAR))
		{
			if (current->prev && token_is_io(current->prev))
			{
				current = current->next;
				continue ;
			}
			else if (current->prev == NULL || current->quote_status == NONE
					&& current->prev != NULL && !token_is_io(current->prev))
				return (current);
			else if (current->prev != NULL && current->quote_status != NONE
					&& !token_is_io(current->prev->prev))
				return (current);
		}
		current = current->next;
	}
	if (token_is_term(current))
		return (current);
	return (NULL);
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

char	*set_assign(t_token *token)
{
	char	*ret;
	int		i;

	ret = ft_strdup(token->value);
	if (!ret)
		return (NULL);
	if (token->near_quote == 0)
	{
		return (ret);
	}
	i = 0;
	while (i <= 3)
	{
		ret = ft_strjoin(ret, token->next->value);
		token = token->next;
		token->token_type = CMD_ASSIGN;
		i++;
	}
	return (ret);
}

int	handle_assign(t_data *data, t_token *token, t_cmd *cmd)
{
	t_token	*current;
	t_env	*env;
	char	*tmp;

	current = get_cmd_first(token);
	if (current && (!current->prev || (current->prev
				&& token_is_term(current->prev))) && is_assign(current->value))
	{
		tmp = set_assign(current);
		if (!tmp)
			return (EXIT_FAILURE);
		*cmd->assign = ft_lstnew_env("assign", tmp);
		env = *cmd->assign;
		free(tmp);
		current = current->next;
		while (current && is_assign(current->value))
		{
			tmp = set_assign(current);
			env->next = ft_lstnew_env("assign", tmp);
			env = env->next;
			free(tmp);
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}

void	build_cmd_list(t_data *data, t_token *token)
{
	t_token	*current_t;
	t_token	*tmp;
	char	*tmp_assign;

	if (token == NULL)
		current_t = *data->token_root;
	else
		current_t = token;
	while (current_t != NULL)
	{
		tmp = current_t;
		if (current_t && (!current_t->prev || token_is_term(current_t->prev))
			&& (is_assign(current_t->value)
				&& current_t->quote_status != O_PAR))
		{
			current_t = current_t->next;
		}
		else if (current_t)
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
			current_t = current_t->next;
		}
		else
		{
			if (current_t)
				current_t = current_t->next;
		}
	}
}
