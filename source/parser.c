#include "../include/minishell.h"
#include <stdlib.h>

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

	(void)data;
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

	(void)str;
	tmp = ft_wildcard(start->value);
	retokenize(data, tmp, start);
	free(tmp);
	return (start);
}

//	Parses tokens looking for things to expand.
void	parse_token(t_data *data)
{
	t_token	*current;

	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD && current->quote_status != SQUOTE
			&& current->quote_status != O_PAR
			&& current->quote_status != IO_HEREDOC)
		{
			var_expander(data, current->value, current);
			if (current->quote_status == NONE  && current->next && (!current->value || (current->value && current->value[0] == 0)))
			{
				current = current->next;
				lst_del_prev(&current);
				if (current->prev == NULL)
					*data->token_root = current;
			}
			else if (current->quote_status == NONE && current->prev && (!current->value || (current->value && current->value[0] == 0)))
			{
				current = current->prev;
				lst_del_next(&current);
			}
			else if (current->quote_status == NONE && (!current->value || (current->value && current->value[0] == 0)))
			{
				lst_del_token(&current);
				*data->token_root = NULL;
			}
		}
		if (current->token_type == WORD && current->quote_status == NONE && wc_present(current->value))
			current = wc_tokenize(current, current->value, data);
		if (current->token_type == IO_HEREDOC && current->next
			&& ((current->next->token_type == WORD)
				|| (token_is_quote(current->next)
				&& current->next->next->token_type == WORD)))
		{
			if (current->next && (current->next->token_type == WORD))
				current->next->quote_status = DQUOTE;
			else if (current->next && ((current->next->token_type == WORD)
						|| (token_is_quote(current->next)
						&& current->next->next->token_type == WORD)))
				current->next->next->quote_status = SQUOTE;
		}
		current = current->next;
	}
}
