# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/10 23:54:46 by nsierra-          #+#    #+#              #
#    Updated: 2021/12/18 05:16:35 by nsierra-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRC = main.c \
	utils.c \
	cmd.c \
	process.c \
	libft/ft_bzero.c \
	libft/ft_calloc.c \
	libft/ft_memset.c \
	libft/ft_putendl_fd.c \
	libft/ft_putstr_fd.c \
	libft/ft_split.c \
	libft/ft_strlen.c \
	libft/ft_strncmp.c \
	libft/ft_substr.c \
	libft/ft_memcpy.c \
	libft/ft_strlcat.c \
	libft/ft_strdup.c \
	libft/ft_free_strarray.c \

OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror -pedantic -I. -g3

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re
