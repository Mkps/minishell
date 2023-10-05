/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/05 10:34:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define HEREDOC_WARNING_EOF "minishell: warning:i \
here-document delimited by end-of-file (wanted `"

/** Gets the input */

int	here_doc_input(t_data *data, char *limiter, int fd)
{
	char	*str;
	int		flag;

	flag = get_flag(limiter);
	str = readline("heredoc> ");
	while (str)
	{
		str = heredoc_var_expand(data, str, flag);
		if (str == NULL || (!ft_strncmp(str, limiter + flag, (ft_strlen(limiter
							+ flag))) && ft_strlen(str) == ft_strlen(limiter
					+ flag)))
		{
			if (!str)
				return (1);
			return (free_return(0, str, NULL, NULL));
		}
		ft_putendl_fd(str, fd);
		free(str);
		str = readline("heredoc> ");
	}
	if (str && !ft_strncmp(str, limiter + flag, (ft_strlen(limiter + flag))))
		return (free_return(0, str, NULL, NULL));
	if (!str && g_exit_code < 128)
		return (free_return(1, str, NULL, NULL));
	return (-1);
}

void	signal_set_heredoc(void)
{
	signals_here_doc();
	rl_getc_function = getc;
	rl_catch_sigwinch = 0;
	rl_catch_signals = 0;
}

void	handle_sigint(t_data *data, t_io_node *io_node, char *heredoc_tmp)
{
	close (io_node->fd);
	io_node->fd = -2;
	unlink(io_node->filename);
	free(heredoc_tmp);
	io_node->filename = NULL;
	data->exit_status = g_exit_code;
}

// Creates a tmp file to get the heredoc 
// then writes to it
int	here_doc_handler(t_data *data, t_io_node *io_node)
{
	char	*heredoc_tmp;

	signal_set_heredoc();
	heredoc_tmp = generate_heredoc_filename();
	io_node->fd = open(heredoc_tmp, O_CREAT | O_TRUNC | O_WRONLY,
			S_IRUSR | S_IWUSR);
	if (io_node->fd == -1)
		return (output_err_ret(-1, "Error while opening file for heredoc",
				NULL));
	if (here_doc_input(data, io_node->filename, io_node->fd) == 1)
		printf("%s%s')\n", HEREDOC_EOF, io_node->filename);
	rl_getc_function = rl_getc;
	rl_done = 1;
	signals_no_interact();
	close (io_node->fd);
	io_node->fd = 1;
	io_node->filename = heredoc_tmp;
	if (g_exit_code > 128)
	{
		handle_sigint(data, io_node, heredoc_tmp);
		return (-1);
	}
	return (io_node->fd);
}
