/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:07:45 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/17 00:46:53 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	word_size(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (*s && *s != c)
	{
		s++;
		len++;
	}
	return (len);
}

static size_t	count_words(char const *s, char c)
{
	size_t	res;

	res = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			res++;
			while (*s && *s != c)
				s++;
		}
	}
	return (res);
}

static char	**hydrate_words(char **tab, char const *s, char c)
{
	size_t	word;

	word = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			tab[word++] = ft_substr(s, 0, word_size(s, c));
			while (*s && *s != c)
				s++;
		}
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	wcount;

	wcount = count_words(s, c);
	res = ft_calloc(wcount + 1, sizeof(char *));
	if (res == NULL)
		return (NULL);
	return (hydrate_words(res, s, c));
}
