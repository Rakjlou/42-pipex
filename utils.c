/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:03:13 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/19 04:17:50 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "pipex.h"

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
