/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: math <math@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:35:04 by math              #+#    #+#             */
/*   Updated: 2024/11/26 11:27:53 by math             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_free_ptr(char **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

char	*ft_join_and_free(char *text, char *buffer)
{
	char	*temp;

	if (!text || !buffer)
		return (NULL);
	temp = ft_strjoin(text, buffer);
	ft_free_ptr(&text);
	return (temp);
}

char	*read_first_line(int fd, char *text)
{
	char	*buffer;
	int		bytes_read;

	if (!text)
		text = ft_calloc(1, 1);
	if (!text)
		return (NULL);

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
	{
		ft_free_ptr(&text);
		return (NULL);
	}

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			ft_free_ptr(&text);
			ft_free_ptr(&buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		text = ft_join_and_free(text, buffer);
		if (!text)
		{
			ft_free_ptr(&buffer);
			return (NULL);
		}
		if (ft_strchr(text, '\n'))
			break ;
		if (bytes_read == 0)
			break ;
	}
	ft_free_ptr(&buffer);
	return (text);
}

char	*ft_get_line(char *text)
{
	int		i;
	char	*str;

	i = 0;
	if (!text || !text[i])
		return (NULL);
	while (text[i] && text[i] != '\n')
		i++;
	if (text[i] == '\n')
		i++;

	str = ft_calloc(i + 1, 1);
	if (!str)
		return (NULL);
	
	memcpy(str, text, i);
	return (str);
}

char	*clean_first_line(char *text)
{
	int		i;
	int		j;
	char	*str;

	if (!text)
		return (NULL);
	i = 0;
	while (text[i] && text[i] != '\n')
		i++;
	if (text[i] == '\n')
		i++;
	if (!text[i])
	{
		ft_free_ptr(&text);
		return (NULL);
	}
	str = ft_calloc(ft_strlen(text) - i + 1, 1);
	if (!str)
	{
		ft_free_ptr(&text);
		return (NULL);
	}

	j = 0;
	while (text[i])
		str[j++] = text[i++];
	
	ft_free_ptr(&text);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*text[1024];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
	{
		if (fd >= 0 && fd < 1024)
			ft_free_ptr(&text[fd]);
		return (NULL);
	}

	text[fd] = read_first_line(fd, text[fd]);
	if (!text[fd])
	{
		ft_free_ptr(&text[fd]);
		return (NULL);
	}

	line = ft_get_line(text[fd]);
	text[fd] = clean_first_line(text[fd]);
	return (line);
}