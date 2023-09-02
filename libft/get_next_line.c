/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 10:48:55 by aloubier          #+#    #+#             */
/*   Updated: 2023/07/25 05:49:31 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

char	*ft_addstr(char *buffer, char *str)
{
	char	*tmp;

	tmp = ft_strjoin(buffer, str);
	free(buffer);
	return (tmp);
}

char	*ft_getleftover(char *buffer)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		++i;
	if (!buffer[i] || (buffer[i]) == '\n')
	{
		free(buffer);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(buffer) - i + 1), sizeof (char));
	++i;
	j = 0;
	while (buffer[i + j])
	{
		line[j] = buffer[i + j];
		++j;
	}
	free(buffer);
	return (line);
}

char	*ft_getline(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof (char));
	if (!line)
		return (NULL);
	line[i] = 0;
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		++i;
	}
	if (buffer[i] && buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}

char	*read_file(int fd, char *str)
{
	char	*buffer;
	int		byte_read;

	if (!str)
		str = ft_calloc(1, 1);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof (char));
	if (!buffer)
		return (NULL);
	byte_read = 1;
	while (byte_read > 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			free(str);
			free(buffer);
			return (NULL);
		}
		buffer[byte_read] = 0;
		str = ft_addstr(str, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_getline(buffer);
	buffer = ft_getleftover(buffer);
	return (line);
}
