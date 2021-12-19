/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 05:13:49 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/19 05:39:16 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdio.h>

static void	last_process(t_pipex *p, int input)
{
	t_cmd	cmd;
	t_bool	ret;
	int		error;

	ret = load_cmd(p, &cmd);
	substitute_fd(p->dest_fd, STDOUT_FILENO, p);
	substitute_fd(input, STDIN_FILENO, NULL);
	if (ret == true)
	{
		error = execve(cmd.pathname, cmd.argv, cmd.env);
		if (error == -1)
			perror(cmd.raw);
	}
	destroy_cmd(&cmd);
}

static void	first_process(t_pipex *p, int output)
{
	t_cmd	cmd;
	t_bool	ret;
	int		error;

	ret = load_cmd(p, &cmd);
	substitute_fd(p->source_fd, STDIN_FILENO, p);
	substitute_fd(output, STDOUT_FILENO, NULL);
	if (ret == true)
	{
		error = execve(cmd.pathname, cmd.argv, cmd.env);
		if (error == -1)
			perror(cmd.raw);
	}
	destroy_cmd(&cmd);
}

static void	exec_inbetween(t_pipex *p, int output)
{
	t_cmd	cmd;
	t_bool	ret;
	int		error;

	substitute_fd(output, STDOUT_FILENO, NULL);
	ret = load_cmd(p, &cmd);
	if (ret == true)
	{
		error = execve(cmd.pathname, cmd.argv, cmd.env);
		if (error == -1)
			perror(cmd.raw);
	}
	destroy_cmd(&cmd);
}

static void	inbetween_process(t_pipex *p, int input)
{
	substitute_fd(input, STDIN_FILENO, NULL);
	pipex(p, exec_inbetween, dispatch_process);
}

void	dispatch_process(t_pipex *p, int fd)
{
	int	c;

	c = p->current_cmd;
	if (c == 0)
		first_process(p, fd);
	else if (c > 0 && c + 1 < p->cmd_count)
		inbetween_process(p, fd);
	else
		last_process(p, fd);
}
