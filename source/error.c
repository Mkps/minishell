#include "../include/minishell.h"
#include <stdlib.h>

void	output_err(char *msg, t_token *token, int squotes)
{
	char	*name_str;
	char	*tmp_str;
	char	*token_str;

	name_str = "minishell: ";
	if (!token)
	{
		ft_putstr_fd(ft_strappend(name_str, msg, 0), 2);
		return ;
	}
	token_str = token->value;
	tmp_str = ft_strappend(name_str, msg, 0);
	if (squotes == 1)
		tmp_str = ft_strappend(tmp_str, "'", 0);
	tmp_str = ft_strappend(tmp_str, token_str, 0);
	if (squotes == 1)
		tmp_str = ft_strappend(tmp_str, "'", 0);
	tmp_str = ft_strappend(tmp_str, " \n", 0);
	ft_putstr_fd(tmp_str, 2);


}
int	check_io_error(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		if (token_is_io(tmp))
		{
			if (tmp->next && (tmp->next->token_type != WORD && !token_is_quote(tmp->next)))
			{
				if (tmp->next->token_type == TERM_END)
					ft_printf("syntax error near unexpected token `newline'\n");
				if (tmp->next->token_type == PIPE)
					ft_printf("syntax error near unexpected token `|'\n");
				return (EXIT_FAILURE);
			}
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	check_quote_error(t_token **root)
{
	t_token	*tmp;
	int		quote_status;

	quote_status = 0;
	tmp = *root;
	while (tmp)
	{
		if (token_is_quote(tmp) && quote_status == 0)
			quote_status = tmp->token_type;
		else if (quote_status != 0 && (tmp->token_type == quote_status))
			quote_status = 0;
		tmp = tmp->next;
	}
	if (quote_status == 0)
		return (EXIT_SUCCESS);
	if (quote_status == DQUOTE)
		output_err("unexpected EOF while looking for matching `\"\'\n", NULL, 0);
	else if (quote_status == SQUOTE)
		output_err("unexpected EOF while looking for matching `\'\'\n", NULL, 0);
	return (EXIT_FAILURE);
}

int	check_par_error(t_token **root)
{
	t_token *tmp;
	int		par_status;

	par_status= 0;
	tmp = *root;
	while (tmp)
	{
		if (par_status == 0 && tmp->token_type == C_PAR)
			output_err("syntax error near unexpected token `)'", NULL, 0);
		if (tmp->prev && tmp->token_type == O_PAR && !token_is_term(tmp->prev) && tmp->prev->token_type != O_PAR)
		{
			if (!tmp->prev->prev && tmp->prev->token_type == WORD)
				output_err("syntax error near unexpected token ", tmp->next , 1);
			else
				output_err("syntax error near unexpected token `(' \n", NULL, 0);
		}
		tmp = tmp->next;
	}
	return (EXIT_FAILURE);
}

int	check_error(t_data *data)
{
	if (check_io_error(data->token_root) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_quote_error(data->token_root) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (check_par_error(data->token_root) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
