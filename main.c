/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:10:36 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/18 00:51:57 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // REMOVE ME
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "pipex.h"

static int	open_file(const char *filename, int mode)
{
	int	fd;

	fd = open(filename, mode);
	if (fd <= 0)
		return (perror(filename), -1);
	return (fd);
}

static char	**build_path(char **env)
{
	while (*env)
	{
		if (ft_strncmp("PATH=", *env, 5) == 0 && ft_strlen(*env) > 5)
			return (ft_split(*env + 5, ":"));
		env++;
	}
	return (NULL);
}

static t_bool	load_pipex(int ac, char **av, char **env, t_pipex *p)
{
	p->source = av[1];
	p->dest = av[ac - 1];
	p->env = env;
	p->path = build_path(p->env);
	p->dest_mode = O_WRONLY | O_CREAT | O_TRUNC;
	p->source_fd = open_file(p->source, O_RDONLY);
	if (p->source_fd <= 0)
		return (false);
	p->dest_fd = open_file(p->dest, p->dest_mode);
	if (p->dest_fd <= 0)
	{
		if (close(p->source_fd) == -1)
			perror(p->dest);
		return (false);
	}
	p->commands = av + 2;
	return (true);
}

static void	destroy_pipex(t_pipex *p)
{
	int	i;

	if (p->source_fd > 0 && close(p->source_fd) == -1)
		perror(p->source);
	else if (p->dest_fd > 0 && close(p->dest_fd) == -1)
		perror(p->dest);
	else if (p->path != NULL)
	{
		i = 0;
		while (p->path[i])
		{
			free(p->path[i]);
			i++;
		}
		free(p->path);
	}
}

/*
static void	print_pipex(t_pipex *p)
{
	char	**cursor;

	cursor = p->commands;
	printf("Source: %s (%d)\n", p->source, p->source_fd);
	printf("Dest: %s (%d)\n", p->dest, p->dest_fd);
	puts("Commands: [");
	while (*(cursor + 1))
	{
		printf("\t\"%s\"\n", *cursor);
		cursor++;
	}
	puts("]");
}
*/

static void	child_process(int input)
{
	(void)input;
}

static void	first_process(int output, t_pipex *p)
{
	t_cmd	cmd;
	t_bool	ret;

	ret = load_cmd(p, &cmd, p->commands[0]);
	printf(">>>(%d) %s\n", ret, cmd.pathname);
	substitute_fd(p->source_fd, STDIN_FILENO, p);
	substitute_fd(output, STDOUT_FILENO, NULL);
	//execve(p->commands[0], NULL, NULL);
	destroy_cmd(&cmd);
}

static void	pipex(t_pipex *p)
{
	int		pipefd[2];
	pid_t	cpid;

	if (pipe(pipefd) == -1)
	{
		perror("pipex pipe");
		return ;
	}
	cpid = fork();
	if (cpid == -1)
		perror("pipex fork");
	else if (cpid == PIPEX_CHILD)
	{
		close(pipefd[PIPEX_PARENT]);
		child_process(pipefd[PIPEX_CHILD]);
	}
	else
	{
		close(pipefd[PIPEX_CHILD]);
		first_process(pipefd[PIPEX_PARENT], p);
	}
}

int	main(int ac, char **av)
{
	t_pipex		p;
	extern char	**environ;

	(void)pipex;
	if (ac < 5 || !load_pipex(ac, av, environ, &p))
		return (EXIT_FAILURE);
	pipex(&p);
	destroy_pipex(&p);
	return (EXIT_SUCCESS);
}
