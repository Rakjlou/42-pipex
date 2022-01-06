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

static void	print_usage(void)
{
	int	fd;

	fd = STDOUT_FILENO;
	write(fd, "usage:\n", 7);
	write(fd, "\t./pipex infile cmd [cmd...] cmd outfile\n", 41);
	write(fd, "\t./pipex here_doc DELIMITER cmd [cmd...] cmd outfile\n", 53);
}

static t_bool	load_heredoc(int ac, char **av, t_pipex *p)
{
	p->commands = av + 3;
	p->cmd_count = ac - 4;
	p->source = NULL;
	p->heredoc_del = av[2];
	p->dest_oflags = O_WRONLY | O_CREAT | O_APPEND;
	p->source_fd = 0;
	return (true);
}

static t_bool	load_regular(int ac, char **av, t_pipex *p)
{
	p->commands = av + 2;
	p->cmd_count = ac - 3;
	p->source = av[1];
	p->heredoc_del = NULL;
	p->dest_oflags = O_WRONLY | O_CREAT | O_TRUNC;
	p->source_fd = open_file(p->source, O_RDONLY, 0);
	if (p->source_fd <= 0)
		return (false);
	return (true);
}

t_bool	load_pipex(int ac, char **av, char **env, t_pipex *p)
{
	int	hd;

	hd = ac > 1 && ft_strncmp("here_doc", av[1], ft_strlen(av[1])) == 0;
	if ((hd == 1 && ac < 6) || (hd == 0 && ac < 5))
		return (print_usage(), false);
	else if ((hd == 1 && !load_heredoc(ac, av, p)) || (hd == 0 && !load_regular(ac, av, p)))
		return (false);
	p->current_cmd = 0;
	p->dest = av[ac - 1];
	p->env = env;
	p->dest_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	p->dest_fd = open_file(p->dest, p->dest_oflags, p->dest_mode);
	if (p->dest_fd <= 0)
	{
		if (p->source_fd > 0 && close(p->source_fd) == -1)
			perror(p->source);
		return (false);
	}
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
