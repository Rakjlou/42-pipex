/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 05:13:49 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/18 05:20:49 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdio.h>

void	last_process(int input, t_pipex *p)
{
	t_cmd	cmd;
	t_bool	ret;
	int		error;

	ret = load_cmd(p, &cmd, p->commands[1]);
	if (ret == false)
		return ;
	substitute_fd(p->dest_fd, STDOUT_FILENO, p);
	substitute_fd(input, STDIN_FILENO, NULL);
	error = execve(cmd.pathname, cmd.argv, cmd.env);
	if (error == -1)
		perror(cmd.raw);
	destroy_cmd(&cmd);
}

void	first_process(int output, t_pipex *p)
{
	t_cmd	cmd;
	t_bool	ret;
	int		error;

	ret = load_cmd(p, &cmd, p->commands[0]);
	if (ret == false)
		return ;
	substitute_fd(p->source_fd, STDIN_FILENO, p);
	substitute_fd(output, STDOUT_FILENO, NULL);
	error = execve(cmd.pathname, cmd.argv, cmd.env);
	if (error == -1)
		perror(cmd.raw);
	destroy_cmd(&cmd);
}
