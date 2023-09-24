#include "../../include/minishell.h"

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
