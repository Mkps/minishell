#include "../../include/minishell.h"

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

// Adds an empty command for cases like only a redir or an assign are present
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

// Main function to add_cmd to the cmd_list. Currently works straight out ouf the data structure.
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
