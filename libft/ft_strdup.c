/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 00:19:27 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/18 00:19:59 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*res;

	len = ft_strlen(s1);
	res = ft_calloc(sizeof(char), len + 1);
	if (res == NULL)
		return (NULL);
	return (ft_memcpy(res, s1, len));
}
