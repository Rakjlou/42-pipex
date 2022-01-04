/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 05:13:49 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/04 01:15:22 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>

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
