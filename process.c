/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 05:13:49 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/04 00:54:19 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	handle_prev_pipe(int in, int out)
{
	dup2(out, STDIN_FILENO);
	close(out);
	close(in);
}

void	handle_next_pipe(int in, int out)
{
	close(out);
	dup2(in, STDOUT_FILENO);
	close(in);
}

void	close_pipe(int in, int out)
{
	close(in);
	close(out);
}

void	execute_command(t_pipex *p, t_position position)
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
