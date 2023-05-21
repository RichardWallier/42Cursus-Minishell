/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 14:18:09 by rwallier          #+#    #+#             */
/*   Updated: 2022/07/20 14:58:57 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static size_t	set_end(char const *s, char c);

static size_t	set_rows(char const *s, char c);

static char		**free_all(char **result, size_t size);

char	**ft_split2(char const *s, char c)
{
	char	**result;
	size_t	rows;
	size_t	index;
	size_t	end;

	if (!s)
		return (NULL);
	rows = set_rows(s, c);
	result = (char **)malloc(sizeof(char *) * (rows + 1));
	if (!result)
		return (NULL);
	index = 0;
	while (index < rows)
	{
		while (*s && *s == c)
			s++;
		end = set_end(s, c);
		result[index] = ft_substr(s, 0, end);
		if (!result[index])
			return (free_all(result, index));
		s += end + 1;
		index++;
	}
	result[index] = NULL;
	return (result);
}

static char	**free_all(char **result, size_t size)
{
	size_t	index;

	index = 0;
	while (index < size)
		free(result[index++]);
	free(result);
	return (NULL);
}

static size_t	set_end(char const *s, char c)
{
	size_t	end;

	end = 0;
	while (s[end] && s[end] != c)
	{
		if (s[end] == '"')
		{
			end++;
			while (s[end] != '"')
				end++;
		}
		if (s[end] == 39)
		{
			end++;
			while (s[end] != 39)
				end++;
		}
		end++;
	}
	return (end);
}

static size_t	set_rows(char const *s, char c)
{
	size_t	index;
	size_t	rows;

	rows = 1;
	index = 0;
	while (*s && *s == c)
		s++;
	while (s[index++])
	{
		if (s[index - 1] == c && s[index] != c)
			rows++;
		if (s[index] == 39)
		{
			index++;
			while (s[index] != 39)
				index++;
		}
		if (s[index] == '"')
		{
			index++;
			while (s[index] != '"')
				index++;
		}
	}
	return (rows);
}
