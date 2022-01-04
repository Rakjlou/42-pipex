/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 01:02:22 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/04 01:09:25 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

t_bool	load_pipex(int ac, char **av, char **env, t_pipex *p)
{
	p->current_cmd = 0;
	p->cmd_count = ac - 3;
	p->source = av[1];
	p->dest = av[ac - 1];
	p->env = env;
	p->dest_oflags = O_WRONLY | O_CREAT | O_TRUNC;
	p->dest_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	p->source_fd = open_file(p->source, O_RDONLY, 0);
	if (p->source_fd <= 0)
		return (false);
	p->dest_fd = open_file(p->dest, p->dest_oflags, p->dest_mode);
	if (p->dest_fd <= 0)
	{
		if (close(p->source_fd) == -1)
			perror(p->source);
		return (false);
	}
	p->commands = av + 2;
	p->path = build_path(p->env);
	return (true);
}

void	destroy_pipex(t_pipex *p)
{
	int	i;

	if (p->source_fd > 0 && close(p->source_fd) == -1)
		perror(p->source);
	else if (p->dest_fd > 0 && close(p->dest_fd) == -1)
		perror(p->dest);
	else if (p->path != NULL)
	{
		i = 0;
		while (p->path[i])
		{
			free(p->path[i]);
			i++;
		}
		free(p->path);
	}
}
