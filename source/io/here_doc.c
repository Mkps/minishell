/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/26 16:24:59 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	here_doc_input(t_data *data, char *limiter, int fd)
{
	char	*str;
	int		flag;

	flag = get_flag(limiter);
	str = readline("heredoc> ");
	while (str)
	{
		if (!flag)
			str = heredoc_var_expand(data, str);
		if (str == NULL || (!ft_strncmp(str, limiter + flag, (ft_strlen(limiter
							+ flag))) && ft_strlen(str) == ft_strlen(limiter
					+ flag)))
		{
			if (!str)
				return (1);
			free(str);
			return (0);
		}
		ft_putendl_fd(str, fd);
		free(str);
		str = readline("heredoc> ");
	}
	if (str && !ft_strncmp(str, limiter + flag, (ft_strlen(limiter + flag))))
	{
		free(str);
		return (0);
	}
	if (!str && g_exit_code < 128)
		return (1);
	return (-1);
}

// Generates a unique heredoc tmp filename
char	*generate_heredoc_filename(void)
{
	char		*filename;
	char		*basename;
	char		*dirpath;
	int			attempt;
	struct stat	buffer;

	basename = "/heredoc-tmp";
	attempt = 0;
	stat("/tmp", &buffer);
	if (!(buffer.st_mode & (S_IRUSR)) || !(buffer.st_mode & (S_IWUSR)))
		dirpath = ft_strappend(".", basename, 0);
	else
		dirpath = ft_strappend("/tmp", basename, 0);
	while (attempt < 5)
	{
		filename = ft_strappend(dirpath, "_", 0);
		filename = ft_strappend(filename, ft_itoa(attempt), 3);
		if (filename == NULL)
		{
			perror("minishell: allocation error:");
			return (NULL);
		}
		if (stat(filename, &buffer) == -1)
		{
			free(dirpath);
			return (filename);
		}
		free(filename);
		attempt++;
	}
	ft_putstr_fd("minishell: Error unable to generate a \
heredoc after 5 tries.\n", 2);
	free(dirpath);
	return (NULL);
}

// Creates a tmp file to get the heredoc 
// then writes to it
int	here_doc_handler(t_data *data, t_io_node *io_node)
{
	char	*heredoc_tmp;

	signals_here_doc();
	// rl_getc_function = getc;
	rl_catch_sigwinch = 0;
	rl_catch_signals = 0;
	heredoc_tmp = generate_heredoc_filename();
	io_node->fd = open(heredoc_tmp, O_CREAT | O_TRUNC | O_WRONLY,
			S_IRUSR | S_IWUSR);
	if (io_node->fd == -1)
		return (output_err_ret(-1, "Error while opening file for heredoc",
				NULL));
	if (here_doc_input(data, io_node->filename, io_node->fd) == 1)
		printf("%s%s')\n", HEREDOC_EOF, io_node->filename);
	// rl_getc_function = rl_getc;
	// rl_done = 1;
	signals_no_interact();
	io_node->fd = open_fd(0, heredoc_tmp);
	io_node->filename = heredoc_tmp;
	if (g_exit_code > 128)
	{
		close(io_node->fd);
		unlink(io_node->filename);
		return (-1);
	}
	return (io_node->fd);
}
