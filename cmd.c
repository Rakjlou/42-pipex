/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 00:05:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/17 01:37:35 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static char	*build_pathname(const char *path, const char *name)
{
	char	*pathname;
	size_t	total;
	size_t	total_path;
	size_t	total_name;

	total_path = ft_strlen(path);
	total_name = ft_strlen(name);
	total = 1 + total_path + total_name;
	if (total_path > 0 && path[total_path - 1] != '/')
		total += 1;
	pathname = ft_calloc(sizeof(char), total + 1);
	if (pathname == NULL)
		return (NULL);
	ft_strlcat(pathname, path, total);
	if (total_path > 0 && path[total_path - 1] != '/')
		ft_strlcat(pathname, "/", total);
	ft_strlcat(pathname, name, total);
	return (pathname);
}

t_bool	load_cmd(t_pipex *p, t_cmd *cmd, char *raw)
{
	int		i;
	char	*pathname;

	cmd->raw = raw;
	cmd->env = p->env;
	cmd->argv = ft_split(raw, ' '); // Huge flaw, should split on all spaces
	if (cmd->argv == NULL)
		return (false);
	i = 0;
	while (p->path[i])
	{
		pathname = build_pathname(p->path[i], cmd->argv[0]);
		if (access(pathname, X_OK) == 0)
		{
			cmd->pathname = pathname;
			return (true);
		}
		free(pathname);
		i++;
	}
	// Command not found
	return (false);
}
