/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_strarray.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 00:33:07 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/18 00:35:40 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_free_strarray(char ***array)
{
	int		i;
	char	**tofree;

	i = 0;
	tofree = *array;
	while (tofree[i])
	{
		free(tofree[i]);
		++i;
	}
	free(tofree);
	*array = NULL;
}
