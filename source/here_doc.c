/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:08 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 01:07:13 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

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

void	here_doc_input(t_data *data, char *limiter, int *fd)
{
	char	*str;
	char	**envv;
	char	*tmp;
	int		flag;

	close(fd[0]);
	signals_here_doc_child();
	signal(SIGQUIT, (void (*)(int))here_doc_child_SIGINT);
	here_doc_child_SIGINT(42, fd, data);
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
			free(limiter);
			close(fd[1]);
			if (str)
			{
				free(str);
				free_child(data);
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
	free_child(data);
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
		here_doc_input(data, ft_strdup(limiter), p_fd);
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
