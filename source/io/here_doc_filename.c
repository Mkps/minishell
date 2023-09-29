/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_filename.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 12:27:06 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 17:13:22 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*generate_dirpath(void)
{
	char		*basename;
	char		*dirpath;
	struct stat	buffer;

	basename = "/heredoc-tmp";
	stat("/tmp", &buffer);
	if (!(buffer.st_mode & (S_IRUSR)) || !(buffer.st_mode & (S_IWUSR)))
		dirpath = ft_strappend(".", basename, 0);
	else
		dirpath = ft_strappend("/tmp", basename, 0);
	return (dirpath);
}

char	*generate_filename(char *dirpath, int attempt)
{
	char		*filename;
	struct stat	buffer;

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
	free(dirpath);
	return (NULL);
}

// Generates a unique heredoc tmp filename
char	*generate_heredoc_filename(void)
{
	char		*filename;
	char		*dirpath;
	int			attempt;

	attempt = 0;
	while (attempt < 5000)
	{
		dirpath = generate_dirpath();
		filename = generate_filename(dirpath, attempt);
		if (filename)
			return (filename);
		attempt++;
	}
	ft_putstr_fd("minishell: Error unable to generate a \
heredoc after 5 tries.\n", 2);
	free(dirpath);
	return (NULL);
}
