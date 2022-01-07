/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 00:57:45 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/07 21:45:49 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
		dup2(p->pipe[in], STDOUT_FILENO);
		close(p->pipe[out]);
		if (p->heredoc_del == NULL)
			execute_command(p, first);
		else
			(heredoc(p), destroy_pipex(p), exit(EXIT_SUCCESS));
	}
	else
	{
		close(p->pipe[in]);
		if (p->heredoc_del != NULL)
			wait(NULL);
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
		dup2(p->pipe[out], STDIN_FILENO);
		close(p->pipe[out]);
		execute_command(p, last);
	}
	else
		close(p->pipe[out]);
}

static void	pipex_middle(t_pipex *p)
{
	int		prev_out;
	pid_t	cpid;

	prev_out = p->pipe[out];
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
		dup2(prev_out, STDIN_FILENO);
		dup2(p->pipe[in], STDOUT_FILENO);
		close(prev_out);
		close(p->pipe[out]);
		execute_command(p, middle);
	}
	else
	{
		close(prev_out);
		close(p->pipe[in]);
	}
}

void	exec_all(t_pipex *p)
{
	int		status;
	int		exit_status;

	exit_status = EXIT_SUCCESS;
	pipex_first(p);
	if (p->heredoc_del == NULL)
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
