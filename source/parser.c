#include "../include/minishell.h"

char	*str_env(t_data *data, char *str)
{
	int		i;
	char	*tmp;
	char	**env;

 tmp = 0;
	i = 0;
	env = data->envv;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}
//Returns the value of the var. NULL if not set.
char	*get_var(t_data *data,char *str)
{
	int		i;
	char	*ret;
	char	*env;
	char	**env_p;
	char	*tmp;

	printf ("looking for %s\n", str);
	env = ft_getenv(data->envv, str);
	if (env == NULL)
	{
		free(env);
		return (NULL);
	}
	return (env);
}

// Replaces the string at r_index of length n by str
char	*str_replace(char *src, int r_index, int n, char *str)
{
	int	i;
	int		ret_len;
	int		str_len;
	int		last_index;
	char	*ret;
	
	if (str == NULL)
		str_len = 0;
	else
		str_len = ft_strlen(str);
	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	ret_len = ft_strlen(src) + (last_index - r_index) + str_len; 
	ret = (char *)malloc(sizeof (char) * (ret_len + 1));
	if (!ret)
		printf("error allocating mem for return string\n");
	i = 0;
	while (i < r_index)
	{
		ret[i] = src[i];
		i++;
	}
	while (str && *str)
		ret[i++] = *str++;
	while (i < ret_len && src[last_index])
		ret[i++] = src[last_index++];
	ret[i] = 0;
	return (ret);
}

// Replaces the $VAR with its' corresponding value stored in env if it exists.
char	*var_expander(t_data *data, char *str)
{
	int	i;
	int	n;
	
	i = 0;
	n = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			while (*(str + i + n) && ft_get_sep_type(str + i + n) == WORD)
				n++;
			return (var_expander(data, str_replace(str, i, n, get_var(data, ft_str_extract(str + i + 1, n - 1)))));	
		}
		if (str[i] == 92)
		{
			if (str[i + 1] && str[i + 1] == 34)
			 return (var_expander(data, str_replace(str, i, 2, "\"")));
		}
		i++;
	}
	return (str);
}

void	parse_token(t_data *data)
{
	t_token	*current;

	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD)
		{
			current->value = var_expander(data, current->value);	
		}
		current = current->next;
	}
}

// t_cmd	*set_io_cmd(t_cmd *cmd)
// {
// }
t_cmd	*create_cmd(t_data *data)
{
	t_cmd	*ret;

	ret = (t_cmd *)malloc(sizeof(t_cmd) * 1);
	if (!ret)
		return (NULL); 
	ret->next = NULL;
	return (ret);
}
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
}

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

t_token	*add_cmd(t_data *data, t_token *token)
{
	t_cmd	*new_cmd;
	t_token	*current;
	int		type;
	char	*tmp;
	char	**args;

	current = token;
	add_cmd_back(data);
	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = token->value; 
	tmp = ";";
	while (current->token_type == WORD )
	{
		tmp = ft_strjoin(tmp, ";");
		tmp = ft_strjoin(tmp, current->value);	
		if (current->next->token_type == OSQUOTE || current->next->token_type == ODQUOTE)
		{
			type = current->next->token_type;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	new_cmd->args = ft_split(tmp, ';');
	// printf("strtmp %s\n", tmp);
	// for (int i = 0; new_cmd->args[i]; i++)
	// 	printf("arg is %s\n", new_cmd->args[i]);
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
void	build_cmd_list(t_data *data, t_token *token)
{
	t_token	*current_t;

	if (token == NULL)
		current_t  = *data->token_root;
	else
		current_t = token;
	if (current_t != NULL)
	{
		if (is_assign(current_t->value))
			add_assign_cmd(data);
		else if (!is_assign(current_t->value))
			current_t = add_cmd(data, current_t);
		else
		 	current_t = current_t->next;
	}

}
