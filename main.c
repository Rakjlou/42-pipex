/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:10:36 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/19 05:31:32 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"

static void	destroy_pipex(t_pipex *p)
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

static t_bool	load_pipex(int ac, char **av, char **env, t_pipex *p)
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

void	pipex(t_pipex *p,
	void (*parent)(t_pipex *, int),
	void (*child)(t_pipex *, int))
{
	int		pipefd[2];
	pid_t	cpid;

	if (pipe(pipefd) == -1)
	{
		perror("pipex pipe");
		return ;
	}
	cpid = fork();
	if (cpid == -1)
		perror("pipex fork");
	else if (cpid == PIPEX_CHILD)
	{
		p->current_cmd++;
		close(pipefd[PIPEX_PARENT]);
		child(p, pipefd[PIPEX_CHILD]);
	}
	else
	{
		close(pipefd[PIPEX_CHILD]);
		parent(p, pipefd[PIPEX_PARENT]);
	}
}

int	main(int ac, char **av)
{
	t_pipex		p;
	extern char	**environ;

	if (ac < 5 || !load_pipex(ac, av, environ, &p))
		return (EXIT_FAILURE);
	pipex(&p, dispatch_process, dispatch_process);
	destroy_pipex(&p);
	return (EXIT_FAILURE);
}
