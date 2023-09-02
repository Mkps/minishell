/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/08/04 09:04:23 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/** Get the input */
void	here_doc_input(t_pipex *p, char *limiter, int *fd)
{
	char	*str;

	close(fd[0]);
	str = "str";
	while (str)
	{
		ft_putstr_fd(">", 1);
		str = get_next_line(0);
		if (str == NULL || (!ft_strncmp(str, limiter, ft_strlen(limiter)) 
				&& (ft_strlen(limiter) == ft_strlen(str) - 1)))
		{
			free(str);
			free(p);
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

void	here_doc_handler(char *limiter, t_pipex *p)
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
		here_doc_input(p, limiter, p_fd);
	else
	{
		close(p_fd[1]);
		waitpid(pid, &status, 0);
		if (status != 256)
			ft_printf("pipex: warning: here-doc end /w EOF(wanted `%s').\n",
				limiter);
		dup2(p_fd[0], 0);
		close(p_fd[0]);
	}
}
