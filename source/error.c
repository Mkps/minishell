#include "../include/minishell.h"
#include <stdlib.h>

//returns the token as a str
// char	*get_str_token(int token_type)
// {
// 	if (token_type == PIPE)
// 		return ("|");
// 	if (token_type == TERM_END)
// 		return ("newline");
// 	if (token_type == IO_APPEND)
// 		return (">");
// 	if (token_type == IO_INPUT)
// 		return ("<");
// 	if (token_type == IO_APPEND)
// 		return (">>");
// 	if (token_type == IO_HEREDOC)
// 		return ("<<");
// 	if (token_type == TERM_SC)
// 		return (";");
// }
void	output_err(char *msg, t_token *token, int squotes)
{
	char	*name_str;
	char	*tmp_str;
	char	*token_str;

	name_str = "minishell: ";
	if (!token)
	{
		ft_putendl_fd(ft_strappend(name_str, msg, 0), 2);
		return ;
	}
	token_str = token->value;
	if (token->token_type == TERM_END)
		token_str = "newline";
	tmp_str = ft_strappend(name_str, msg, 0);
	if (squotes == 1)
		tmp_str = ft_strappend(tmp_str, "'", 2);
	tmp_str = ft_strappend(tmp_str, token_str, 2);
	if (squotes == 1)
		tmp_str = ft_strappend(tmp_str, "'", 2);
	tmp_str = ft_strappend(tmp_str, "\n", 2);
	ft_putstr_fd(tmp_str, 2);
	free(tmp_str);


}
void	output_err_cmd(char *msg, char *cmd_str)
{
	char	*name_str;
	char	*tmp_str;

	name_str = PROG_NAME;
	if (!cmd_str)
	{
		ft_putendl_fd(ft_strappend(name_str, msg, 0), 2);
		return ;
	}
	tmp_str = ft_strappend(name_str, cmd_str, 0);
	tmp_str = ft_strappend(tmp_str, ": ", 2);
	tmp_str = ft_strappend(tmp_str, msg, 2);
	tmp_str = ft_strappend(tmp_str, "\n", 2);
	ft_putstr_fd(tmp_str, 2);
	free(tmp_str);
}
int	is_wc(char *str)
{
	while (*str)
		if (*str++ == '*') return (1);
	return (0);
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
				output_err("syntax error near unexpected token ", tmp->next, 1);
				return (SYNTAX_ERROR);
			}
			if (tmp->next && (tmp->next->token_type == WORD && is_wc(tmp->next->value)))
			{
				output_err_cmd("ambiguous redirect", tmp->next->value);
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
		output_err("unexpected EOF while looking for matching `\"\'", NULL, 0);
	else if (quote_status == SQUOTE)
		output_err("unexpected EOF while looking for matching `\'\'", NULL, 0);
	return (EXIT_FAILURE);
}

int	check_term_error(t_token **root)
{
	t_token	*tmp;

	tmp = *root;
	while (tmp)
	{
		if (token_is_term(tmp) && tmp->token_type != TERM_END && !tmp->prev)
		{
			output_err("syntax error near unexpected token ", tmp, 1);
			return (EXIT_FAILURE);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	check_par_error(t_token **root)
{
	t_token *tmp;
	int		par_status;

	par_status= 0;
	tmp = *root;
	while (tmp)
	{
		if (tmp->token_type == O_PAR)
		{
			par_status++;
			tmp = tmp->next;
			while (tmp && par_status > 0)
			{
				if (tmp->token_type == O_PAR)
					par_status++;
				if (tmp->token_type == C_PAR)
					par_status--;
				tmp = tmp->next;
				if (!tmp && par_status != 0)
				{
					output_err("unexpected EOF while looking for matching ')'", NULL, 0);
					return (EXIT_FAILURE);
				}
		}
		}
		if (par_status == 0 && tmp->token_type == C_PAR)
		{
			output_err("syntax error near unexpected token `)'", NULL, 0);
			return (EXIT_FAILURE);
		}
		if ((tmp->prev && !token_is_term(tmp->prev)) && tmp->token_type == O_PAR 
				&& !token_is_term(tmp->prev) && tmp->prev->token_type != O_PAR)
		{
			if (!tmp->prev->prev && tmp->prev->token_type == WORD)
				output_err("syntax error near unexpected token ", tmp->next , 1);
			else
				output_err("syntax error near unexpected token `('", NULL, 0);
			return (EXIT_FAILURE);
		}
		if (tmp->token_type == C_PAR)
		{
			if (tmp->prev->token_type == O_PAR)
			{
				output_err("syntax error near unexpected token `)'", NULL, 0);
				return (EXIT_FAILURE);
			}
			par_status--;
		}
		tmp = tmp->next;
	}
	if (par_status != 0)
	{
		output_err("unexpected EOF while looking for matching ')'", NULL, 0);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_error(t_data *data)
{
	int	err;

	err = EXIT_SUCCESS;
	if (!err)
		err = check_io_error(data->token_root);
	if (!err && check_quote_error(data->token_root) == EXIT_FAILURE)
		err = SYNTAX_ERROR;
	if (!err && check_par_error(data->token_root) == EXIT_FAILURE)
		err = SYNTAX_ERROR;
	if (!err && check_term_error(data->token_root) == EXIT_FAILURE)
		err = SYNTAX_ERROR;
	if (err)
		g_exit_code = err;
	return (err);
}
