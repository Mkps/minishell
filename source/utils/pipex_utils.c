/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:23:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 14:39:33 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//Use perror to display the errno and exit with the specified code.
void	error_exit(int exit_code)
{
	perror("minishell: an error occured.");
	exit(exit_code);
}

char	*ft_readline(char *str)
{
	char	*tmp;
	char	*ret;

	ft_printf("%s", str);
	tmp = get_next_line(0);
	if (!tmp)
		return (NULL);
	if (ft_strlen(tmp) > 1)
		ret = ft_substr(tmp, 0, ft_strlen(tmp) - 1);
	else
		ret = ft_strdup(tmp);
	free(tmp);
	return (ret);
}

int	open_fd(int mode, char *filename)
{
	int	fd;

	fd = -3;
	if (filename[0] == 4)
	{
		output_err("ambiguous redirect", NULL, 0);
		return (-1);
	}
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
	if (fd == -3)
		output_err("supported mode", NULL, 0);
	return (fd);
}
