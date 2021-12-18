/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:07:45 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/17 23:47:05 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_bool	char_is_in_str(char c, const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (true);
		++i;
	}
	return (false);
}

static size_t	word_size(char const *s, const char *sep)
{
	size_t	len;

	len = 0;
	while (*s && !char_is_in_str(*s, sep))
	{
		s++;
		len++;
	}
	return (len);
}

static size_t	count_words(char const *s, const char *sep)
{
	size_t	res;

	res = 0;
	while (*s)
	{
		while (char_is_in_str(*s, sep))
			s++;
		if (*s)
		{
			res++;
			while (*s && !char_is_in_str(*s, sep))
				s++;
		}
	}
	return (res);
}

static char	**hydrate_words(char **tab, char const *s, const char *sep)
{
	size_t	word;

	word = 0;
	while (*s)
	{
		while (char_is_in_str(*s, sep))
			s++;
		if (*s)
		{
			tab[word++] = ft_substr(s, 0, word_size(s, sep));
			while (*s && !char_is_in_str(*s, sep))
				s++;
		}
	}
	return (tab);
}

char	**ft_split(char const *str, const char *sep)
{
	char	**res;
	size_t	wcount;

	wcount = count_words(str, sep);
	res = ft_calloc(wcount + 1, sizeof(char *));
	if (res == NULL)
		return (NULL);
	return (hydrate_words(res, str, sep));
}
