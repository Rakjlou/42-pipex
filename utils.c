/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:03:13 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/07 21:51:57 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void	substitute_fd(int fd1, int fd2, t_pipex *p)
{
	dup2(fd1, fd2);
	close(fd1);
	if (p != NULL && fd1 == p->source_fd)
		p->source_fd = -1;
	else if (p != NULL && fd1 == p->dest_fd)
		p->dest_fd = -1;
}

int	open_file(const char *filename, int oflags, int mode)
{
	int	fd;

	fd = open(filename, oflags, mode);
	if (fd <= 0)
		return (perror(filename), -1);
	return (fd);
}

char	**build_path(char **env)
{
	while (*env)
	{
		if (ft_strncmp("PATH=", *env, 5) == 0 && ft_strlen(*env) > 5)
			return (ft_split(*env + 5, ":"));
		env++;
	}
	return (NULL);
}

void	heredoc(t_pipex *p)
{
	char	*line;
	size_t	len;

	while (42)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		if (len > 1 && ft_strncmp(p->heredoc_del, line, len - 1) == 0)
		{
			free(line);
			break ;
		}
		write(STDOUT_FILENO, line, len);
		free(line);
	}
}
