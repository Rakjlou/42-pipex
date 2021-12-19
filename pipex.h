/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 17:38:46 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/19 05:31:53 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stddef.h>

# define PIPEX_PARENT 1
# define PIPEX_CHILD 0

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef struct s_pipex
{
	const char	*source;
	const char	*dest;
	char		**commands;
	char		**env;
	char		**path;
	int			current_cmd;
	int			cmd_count;
	int			source_fd;
	int			dest_fd;
	int			dest_oflags;
	int			dest_mode;
}	t_pipex;

typedef struct s_cmd
{
	char	*raw;
	char	*pathname;
	char	**argv;
	char	**env;
}	t_cmd;

// libft
size_t	ft_strlen(const char *str);
void	ft_putstr_fd(const char *str, int fd);
void	ft_putendl_fd(const char *str, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_bzero(void *s, size_t n);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_calloc(size_t count, size_t size);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s1);
char	**ft_split(char const *str, const char *sep);
void	ft_free_strarray(char ***array);

//process.c
//void	last_process(t_pipex *p, int input);
//void	first_process(t_pipex *p, int output);
void	dispatch_process(t_pipex *p, int fd);

// cmd.c
t_bool	load_cmd(t_pipex *p, t_cmd *cmd);
void	destroy_cmd(t_cmd *cmd);

// utils.c
void	substitute_fd(int fd1, int fd2, t_pipex *p);
int		open_file(const char *filename, int oflags, int mode);
char	**build_path(char **env);

void	pipex(t_pipex *p,
			void (*parent)(t_pipex *, int),
			void (*child)(t_pipex *, int));

#endif
