/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/20 09:25:58 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

#define HEREDOC_WARNING_EOF "minishell: warning: here-document delimited by end-of-file (wanted `"
/** Gets the input */
void	here_doc_input(t_data *data, char *limiter, int *fd)
{
	char	*str;
	char	*tmp;

	signals_here_doc_child();
	signal(SIGQUIT, (void (*) (int))here_doc_child_SIGINT);
	here_doc_child_SIGINT(42, fd);
	close(fd[0]);
	str = "str";
	while (str)
	{
		str = readline("heredoc> ");
		if (str)
			str = ft_strappend(str, "\n", 2);
		if (str == NULL || (!ft_strncmp(str, limiter, ft_strlen(limiter)) 
				&& (ft_strlen(limiter) == ft_strlen(str) - 1)))
		{
			free(limiter);
			close(fd[1]);
			if (str)
			{
				free(str);
				exit(0);
			}
			write(1, "\n", 1);
			exit(2);

		}
		ft_putstr_fd(str, fd[1]);
		free(str);
	}
	close(fd[1]);
	free(str);
}

// Creates a child process to get the heredoc and then duplicates the read end of the pipe on the STDIN_FILENO
int	here_doc_handler(t_data *data, char *limiter)
{
	int		p_fd[2];
	pid_t	pid;
	int		ret_fd;
	int		status;

	if (pipe(p_fd) == -1)
		error_exit(5);
	pid = fork();
	if (pid == -1)
		error_exit(6);
	status = 0;
	if (!pid)
	{
		close(data->old_fd[0]);
		close(data->old_fd[1]);
		close_pipes(data->cmd_list, NULL, NULL);
		here_doc_input(data, limiter, p_fd);
	}
	else
	{
		signals_here_doc();
		close(p_fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 3 || WEXITSTATUS(status) == 130)
			{
				close(p_fd[0]);
				dup2(data->old_fd[0], STDIN_FILENO);
				signals_no_interact();
				return (-1);
			}
		}
		if (WEXITSTATUS(status) == 2)
			ft_printf("%s%s')\n", HEREDOC_WARNING_EOF, limiter);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		signals_no_interact();
		return (0);
	}
	return (-1);
}
