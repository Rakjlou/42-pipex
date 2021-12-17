/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 01:02:39 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/17 01:02:52 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*cdest;
	const char	*csrc;

	if ((dest == NULL && src == NULL) || n == 0)
		return (dest);
	cdest = (char *)dest;
	csrc = (const char *)src;
	while (n-- > 0)
		*cdest++ = *csrc++;
	return (dest);
}
