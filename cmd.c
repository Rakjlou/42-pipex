/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 00:05:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/19 04:08:44 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static char	*build_pathname(const char *path, const char *cmd)
{
	char	*pathname;
	size_t	total;
	size_t	total_path;
	size_t	total_cmd;

	total_path = ft_strlen(path);
	total_cmd = ft_strlen(cmd);
	total = 1 + total_path + total_cmd;
	if (total_path > 0 && path[total_path - 1] != '/')
		total += 1;
	pathname = ft_calloc(sizeof(char), total);
	if (pathname == NULL)
		return (NULL);
	ft_strlcat(pathname, path, total);
	if (total_path > 0 && path[total_path - 1] != '/')
		ft_strlcat(pathname, "/", total);
	ft_strlcat(pathname, cmd, total);
	return (pathname);
}

static char	*get_direct_pathname(char *cmd)
{
	if (access(cmd, X_OK) != 0)
		return (NULL);
	return (ft_strdup(cmd));
}

static char	*get_pathname(char **path, char *cmd)
{
	int		i;
	char	*candidate;

	i = 0;
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (get_direct_pathname(cmd));
	else if (path != NULL)
	{
		while (path[i])
		{
			candidate = build_pathname(path[i], cmd);
			if (access(candidate, X_OK) == 0)
				return (candidate);
			free(candidate);
			++i;
		}
	}
	return (NULL);
}

void	destroy_cmd(t_cmd *cmd)
{
	if (cmd->argv != NULL)
		ft_free_strarray(&cmd->argv);
	if (cmd->pathname != NULL)
		free(cmd->pathname);
}

t_bool	load_cmd(t_pipex *p, t_cmd *cmd, char *raw)
{
	cmd->raw = raw;
	cmd->env = p->env;
	cmd->argv = ft_split(raw, " \t\n\v\f\r");
	if (cmd->argv == NULL)
		return (false);
	cmd->pathname = get_pathname(p->path, cmd->argv[0]);
	if (cmd->pathname == NULL)
	{
		ft_putstr_fd("command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd->argv[0], STDERR_FILENO);
		ft_free_strarray(&cmd->argv);
		return (false);
	}
	return (true);
}
