/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 01:24:39 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/17 01:24:48 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	ft_strnlen(const char *str, size_t limit)
{
	size_t	i;

	i = 0;
	while (*str && i < limit)
	{
		str++;
		i++;
	}
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	size_src;
	size_t	size_dst;

	size_src = ft_strlen(src);
	size_dst = ft_strnlen(dst, size);
	i = 0;
	j = 0;
	if (size == 0)
		return (size_src);
	else if (size <= size_dst)
		return (size + size_src);
	while (dst[i])
		i++;
	while (src[j] != '\0' && (i + j) < size - 1)
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (size_src + size_dst);
}
