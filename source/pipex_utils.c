/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:23:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 11:31:47 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	argc_error(int error_code)
{
	ft_putendl_fd("pipex: error: incorrect number of arguments.", 2);
	if (error_code == 0)
		ft_putendl_fd("Standard use is ./pipex infile cmd cmd outfile", 2);
	if (error_code == 1)
	{
		ft_putstr_fd("Use with here_doc is ", 2);
		ft_putendl_fd("./pipex here_doc LIMITER cmd cmd1 file", 2);
	}
	exit(1);
}

//Use perror to display the errno and exit with the specified code.
void	error_exit(int exit_code)
{
	perror("minishell: an error occured.");
	exit(exit_code);
}

/** Get the env variable from envp specified by str. */
char	*ft_getenv(char **env, const char *str)
{
	int		i;
	char	*tmp;

	tmp = 0;
	i = 0;
	while (env && env[i])
	{
		// printf("listing env i %i = %s\n", i, env[i]);
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			if (*(env[i] + ft_strlen(str)) == '=')
				tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}

char	**ft_strsdup(char	**strs)
{
	int		i;
	char	**ret;
	
	i = 0;
	while (strs[i] != 0) i++;
	if (i == 0)
		return (NULL);
	ret = ft_calloc(i, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		ret[i] = ft_strdup(strs[i]);
		if (!ret[i])
		{
			ft_free_tab(ret);
			free(ret);
			return (NULL);
		}
		i++;
	}
	ret[i] = 0;
	return (ret);
}
int	import_envv(t_data *data, char **envv)
{
	data->envv = ft_strsdup(envv);
	if (!data->envv)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	open_fd(int mode, char *filename)
{
	int	fd;

	if (mode == 0)
		fd = open(filename, O_RDONLY, 0664);
	if (mode == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (mode == 2)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (fd);
	}
	return (fd);
}
