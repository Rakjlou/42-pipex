/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:10:36 by nsierra-          #+#    #+#             */
/*   Updated: 2022/01/04 01:07:49 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdlib.h>

int	main(int ac, char **av)
{
	t_pipex		p;
	extern char	**environ;

	if (ac < 5 || !load_pipex(ac, av, environ, &p))
		return (EXIT_FAILURE);
	exec_all(&p);
	destroy_pipex(&p);
	return (EXIT_FAILURE);
}
