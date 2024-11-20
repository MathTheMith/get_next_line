/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:02:49 by mvachon           #+#    #+#             */
/*   Updated: 2024/11/20 14:32:14 by mvachon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 100

char *get_next_line(int fd)
{
    char buf[BUFFER_SIZE + 1];
    char *result = NULL;
    char *temp = NULL;
    int nb_read;
    size_t total_length = 0;

    if (fd < 0)
        return (NULL);

    while ((nb_read = read(fd, buf, BUFFER_SIZE)) > 0)
    {
        buf[nb_read] = '\0';
        size_t stop_index = 0;
        while (stop_index < (size_t)nb_read && buf[stop_index] != '\n')
            stop_index++;

        total_length += stop_index;
        temp = result;
        result = malloc(total_length + 1);
        if (!result)
            return (NULL);

        if (temp)
        {
            size_t i = 0;
            while (i < total_length - stop_index)
            {
                result[i] = temp[i];
                i++;
            }
            free(temp);
        }

        size_t j = 0;
        while (j < stop_index)
        {
            result[total_length - stop_index + j] = buf[j];
            j++;
        }

        if (stop_index < (size_t)nb_read)
            break;
    }

    if (nb_read < 0)
    {
        free(result);
        return (NULL);
    }

    if (result)
        result[total_length] = '\0';
    return result;
}

int main(void)
{
    int fd = open("doc.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    char *content = get_next_line(fd);
    if (content)
    {
        printf("%s", content);
        free(content);
    }
    close(fd);
    return (0);
}
