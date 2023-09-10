/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 15:42:35 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define HEREDOC_WARNING_EOF "minishell: warning: here-document delimited by end-of-file(wanted `"
/** Gets the input */
void	here_doc_input(char *limiter, int *fd)
{
	char	*str;

	close(fd[0]);
	str = "str";
	while (str)
	{
		ft_printf("heredoc> ");
		str = get_next_line(0);
		if (str == NULL || (!ft_strncmp(str, limiter, ft_strlen(limiter)) 
				&& (ft_strlen(limiter) == ft_strlen(str) - 1)))
		{
			free(str);
			close(fd[1]);
			if (!str)
				exit(2);
			exit(1);
		}
		ft_putstr_fd(str, fd[1]);
		free(str);
	}
	close(fd[1]);
	free(str);
}

// Creates a child process to get the heredoc and then duplicates the read end of the pipe on the STDIN_FILENO
void	here_doc_handler(char *limiter)
{
	int		p_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(p_fd) == -1)
		error_exit(5);
	pid = fork();
	if (pid == -1)
		error_exit(6);
	status = 0;
	if (!pid)
		here_doc_input(limiter, p_fd);
	else
	{
		close(p_fd[1]);
		waitpid(pid, &status, 0);
		if (status != 256)
			ft_printf("%s%s')\n", HEREDOC_WARNING_EOF, limiter);
		dup2(p_fd[0], 0);
		close(p_fd[0]);
	}
}
