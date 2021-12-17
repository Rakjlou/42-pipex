/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:03:13 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/17 00:46:09 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "pipex.h"

void	substitute_fd(int fd1, int fd2, t_pipex *p)
{
	dup2(fd1, fd2);
	close(fd1);
	if (p != NULL && fd1 == p->source_fd)
		p->source_fd = -1;
	else if (p != NULL && fd2 == p->dest_fd)
		p->dest_fd = -1;
}
