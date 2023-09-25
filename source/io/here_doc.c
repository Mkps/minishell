/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/25 18:38:23 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/readline.h>

#define HEREDOC_WARNING_EOF "minishell: warning: here-document delimited by end-of-file (wanted `"

/** Gets the input */
int	get_flag(char *limiter)
{
	int	i;
	int	quote_status;

	quote_status = 0;
	i = -1;
	while (limiter[++i])
	{
		if (limiter[i] == '\'')
			quote_status++;
	}
	return (!(quote_status % 2 == 0));
}

char	*heredoc_var_expand(t_data *data, char *str)
{
	int		i;
	int		n;
	char	*ret;
	char	*tmp;
	char	*tmp_str;
	char	*exit_code;

	i = 0;
	n = 1;
	ret = ft_strdup(str);
	exit_code = ft_itoa(g_exit_code);
	while (ret[i])
	{
		if (ret[i] == '$')
		{
			if (ft_isalpha(ret[i + 1]) || ret[i + 1] == '_')
			{
				n = 1;
				while (*(ret + i + n) && (ft_isalnum(ret[i + n]) || ret[i
						+ n] == '_'))
					n++;
				if (n != 1)
				{
					tmp = ret;
					tmp_str = ft_str_extract(ret + i + 1, n - 1);
					ret = str_replace(ret, i, n, get_var(data, tmp_str));
					free(tmp_str);
					free(tmp);
				}
				i = 0;
			}
			else if (ft_isdigit(ret[i + 1]))
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, "");
				free(tmp);
				i = 0;
			}
			else if (ret[i + 1] == '?')
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, exit_code);
				free(tmp);
				i = 0;
			}
			else
				i++;
		}
		else
			i++;
	}
	free(exit_code);
	free(str);
	return (ret);
}

int	here_doc_input(t_data *data, char *limiter, int fd)
{
	char	*str;
	int		flag;

	flag = get_flag(limiter);
	str = "str";
	while (str)
	{
		str = readline("heredoc> ");
		if (str)
		{
			str = ft_strappend(str, "\n", 2);
			if (!flag)
				str = heredoc_var_expand(data, str);
		}
		if (str == NULL || (!ft_strncmp(str, limiter + flag, ft_strlen(limiter
						+ flag)) && (ft_strlen(limiter + flag) == ft_strlen(str)
					- 1)))
		{
			if (str)
			{
				free(str);
				return (0);
			}
			return (1);
		}
		if (str)
		{
			ft_putstr_fd(str, fd);
			free(str);
		}
	}
	return (-1);
}

// Creates a child process to get the heredoc and then duplicates the read end of the pipe on the STDIN_FILENO
int	here_doc_handler(t_data *data, t_io_node *io_node)
{
	char	*heredoc_tmp;

	signals_here_doc();
	rl_getc_function = getc;
   	rl_catch_sigwinch = 0;
	heredoc_tmp = "./tmp_fd";
	unlink(heredoc_tmp);
	io_node->fd = open(heredoc_tmp, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (io_node->fd == -1)
		return (output_err_ret(-1, "Error while opening file for heredoc", NULL));
	if (here_doc_input(data, io_node->filename, io_node->fd) == 1) 
		printf("here-document delimited by end-of-file (wanted '%s')\n", io_node->filename);
	//rl_clear_signals ();
	rl_getc_function = rl_getc;
	//rl_catch_signals = 1;
	rl_catch_sigwinch = 1;
	//rl_done = 1;
	signals_no_interact();
	close(io_node->fd);
	io_node->fd = open_fd(0, heredoc_tmp);
	io_node->filename = heredoc_tmp;
	if (g_exit_code > 128)
	{
		//close(io_node->fd);
		//unlink(io_node->filename);
		return (-1);
	}
	return (io_node->fd);
}
