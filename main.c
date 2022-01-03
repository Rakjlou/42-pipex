/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:10:36 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/04 00:56:09 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pipex.h"

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

void	pipex_first(t_pipex *p)
{
	pid_t	cpid;

	if (pipe(p->pipe) == -1)
	{
		perror("pipex first pipe");
		return ;
	}
	cpid = fork();
	if (cpid == -1)
		perror("pipex first fork");
	else if (cpid == child)
	{
		handle_next_pipe(p->pipe[in], p->pipe[out]);
		execute_command(p, first);
	}
}

void	pipex_last(t_pipex *p)
{
	pid_t	cpid;

	cpid = fork();
	if (cpid == -1)
		perror("pipex last fork");
	else if (cpid == child)
	{
		handle_prev_pipe(p->pipe[in], p->pipe[out]);
		execute_command(p, last);
	}
	else
		close_pipe(p->pipe[in], p->pipe[out]);
}

void	pipex_middle(t_pipex *p)
{
	int		old_out;
	int		old_in;
	pid_t	cpid;

	old_out = p->pipe[out];
	old_in = p->pipe[in];
	if (pipe(p->pipe) == -1)
	{
		perror("pipex middle pipe");
		return ;
	}
	cpid = fork();
	if (cpid == -1)
		perror("pipex middle fork");
	else if (cpid == child)
	{
		handle_prev_pipe(old_in, old_out);
		handle_next_pipe(p->pipe[in], p->pipe[out]);
		execute_command(p, middle);
	}
	else
		close_pipe(p->pipe[in], p->pipe[out]);
}

void	pipex_supervise(t_pipex *p)
{
	int		status;
	int		exit_status;

	exit_status = EXIT_SUCCESS;
	pipex_first(p);
	p->current_cmd++;
	while (p->current_cmd + 1 < p->cmd_count)
	{
		pipex_middle(p);
		p->current_cmd++;
	}
	pipex_last(p);
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
			exit_status = WEXITSTATUS(status);
	}
	destroy_pipex(p);
	exit(exit_status);
}

int	main(int ac, char **av)
{
	t_pipex		p;
	extern char	**environ;

	if (ac < 5 || !load_pipex(ac, av, environ, &p))
		return (EXIT_FAILURE);
	pipex_supervise(&p);
	destroy_pipex(&p);
	return (EXIT_FAILURE);
}
