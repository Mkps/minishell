/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/08/04 08:59:22 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Get path from the env variable. 
// If envv is (null) set the path to linux defaults
char	**get_path(char **envv)
{
	char	**env_p;
	char	*env;

	env = ft_getenv(envv, "PATH");
	if (env == NULL)
	{
		free(env);
		env = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	}
	env_p = ft_split(env, ':');
	return (env_p);
}

void	fd_heredoc(int argc, char **argv, int *i, t_pipex *p)
{
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")))
	{
		p->here_doc = 1;
		*i = 3;
		if (argc < 6)
			argc_error(1);
		here_doc_handler(argv[2], p);
		p->fd[1] = open_fd(2, argv[argc - 1]);
		if (p->fd[1] == -1)
		{
			free(p);
			exit(1);
		}
	}
}

void	fd_handler(int argc, char **argv, int *i, t_pipex *p)
{
	*i = 2;
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")))
		fd_heredoc(argc, argv, i, p);
	else
	{
		p->here_doc = 0;
		p->fd[1] = open_fd(STDOUT_FILENO, argv[argc - 1]);
		p->fd[0] = open_fd(STDIN_FILENO, argv[1]);
		dup2(p->fd[0], 0);
	}
}

int	main(int argc, char **argv, char **envv)
{
	int		start_index;
	int		i;
	t_pipex	*handler;
	int		status;

	if (argc < 5)
		argc_error(0);
	start_index = 0;
	handler = (t_pipex *)malloc(sizeof(t_pipex));
	fd_handler(argc, argv, &start_index, handler);
	handler->nb_cmd = argc - 1 - start_index;
	pipex_init(handler);
	i = 0;
	while (i < handler->nb_cmd - 1)
	{
		handler->p_arr[i] = (int *)malloc(sizeof(int) * 2);
		if (pipe(handler->p_arr[i]) == -1)
			ft_printf("Error creating pipe %i\n", i);
		i++;
	}
	exec_pipe(handler, &argv[start_index], envv);
	i = 0;
	status = *handler->status;
	free_pipex(handler);
	exit (WEXITSTATUS(status));
}
