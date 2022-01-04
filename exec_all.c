/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 00:57:45 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/04 01:10:24 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

static void	execute_command(t_pipex *p, t_position position)
{
	t_cmd	cmd;
	t_bool	ret;
	int		error;

	ret = load_cmd(p, &cmd);
	if (position == last)
		substitute_fd(p->dest_fd, STDOUT_FILENO, p);
	else if (position == first)
		substitute_fd(p->source_fd, STDIN_FILENO, p);
	if (ret == true)
	{
		error = execve(cmd.pathname, cmd.argv, cmd.env);
		if (error == -1)
			perror(cmd.raw);
	}
	destroy_cmd(&cmd);
	destroy_pipex(p);
	exit(EXIT_FAILURE);
}

static void	pipex_first(t_pipex *p)
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

static void	pipex_last(t_pipex *p)
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

static void	pipex_middle(t_pipex *p)
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

void	exec_all(t_pipex *p)
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
