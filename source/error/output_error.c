/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:21:25 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 11:46:24 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	output_err(char *msg, t_token *token, int squotes)
{
	char	*name_str;
	char	*tmp_str;
	char	*token_str;
	char	tmp_quotes[2];

	ft_memset(tmp_quotes, 0, sizeof (char) * 2);
	if (squotes == 1)
		tmp_quotes[0] = '\'';
	else if (squotes == 2)
		tmp_quotes[0] = '\"';
	name_str = "minishell: ";
	if (!token)
		tmp_str = ft_strappend(name_str, msg, 0);
	else
	{
		token_str = token->value;
		if (token->token_type == TERM_END)
			token_str = "newline";
		tmp_str = ft_strappend(name_str, msg, 0);
		tmp_str = ft_strappend(tmp_str, tmp_quotes, 2);
		tmp_str = ft_strappend(tmp_str, token_str, 2);
		tmp_str = ft_strappend(tmp_str, tmp_quotes, 2);
	}
	ft_putendl_fd(tmp_str, 2);
	free(tmp_str);
}

void	output_err_cmd(char *msg, char *cmd_str)
{
	char	*name_str;
	char	*tmp_str;

	name_str = PROG_NAME;
	if (!cmd_str)
	{
		tmp_str = ft_strappend(name_str, msg, 0);
		ft_putendl_fd(tmp_str, 2);
		free(tmp_str);
		return ;
	}
	tmp_str = ft_strappend(name_str, cmd_str, 0);
	tmp_str = ft_strappend(tmp_str, ": ", 2);
	tmp_str = ft_strappend(tmp_str, msg, 2);
	tmp_str = ft_strappend(tmp_str, "\n", 2);
	ft_putstr_fd(tmp_str, 2);
	free(tmp_str);
}

int	output_err_ret(int return_value, char *msg, char *cmd_str)
{
	char	*name_str;
	char	*tmp_str;

	name_str = PROG_NAME;
	if (!cmd_str)
	{
		tmp_str = ft_strappend(name_str, msg, 0);
		ft_putendl_fd(tmp_str, 2);
		free(tmp_str);
		return (return_value);
	}
	tmp_str = ft_strappend(name_str, cmd_str, 0);
	tmp_str = ft_strappend(tmp_str, ": ", 2);
	tmp_str = ft_strappend(tmp_str, msg, 2);
	tmp_str = ft_strappend(tmp_str, "\n", 2);
	ft_putstr_fd(tmp_str, 2);
	free(tmp_str);
	return (return_value);
}
