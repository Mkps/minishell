/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:23:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/08/04 04:52:03 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>

typedef struct s_pipex {
	int		here_doc;
	int		count;
	int		fd[2];
	int		*status;
	int		nb_cmd;
	pid_t	*pid;
	int		**p_arr;	
}	t_pipex;

char	*ft_getenv(char **env, const char *str);
void	argc_error(int error_code);
void	error_exit(int exit_code);
int		open_fd(int mode, char *filename);
void	here_doc_handler(char *limiter, t_pipex *p);
void	exec_pipe(t_pipex *handler, char **cmd, char **envv);
void	exec_cmd(char *cmd, char **envv);
char	*get_cmd(char *cmd, char **env_p);
char	**get_path(char **envv);
void	last_child(int cmd_index, t_pipex *p, char *cmd, char **envv);
void	middle_child(int cmd_index, t_pipex *p, char *cmd, char **envv);
void	first_child(t_pipex *p, char *cmd, char **envv);
void	parent_handler(t_pipex *p);
void	free_pipex(t_pipex *p);
void	pipex_init(t_pipex *p);

#endif
