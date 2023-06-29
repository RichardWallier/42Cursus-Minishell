/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 21:17:56 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 21:25:02 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_command_count(t_word *node)
{
	int	cmd_count;

	cmd_count = 0;
	while (node && node->flag != MS_PIPE)
	{
		if (node->flag == MS_WORD)
			cmd_count++;
		node = node->next;
	}
	return (cmd_count);
}

int	get_environment_len_after_expand(char *line, t_list *env)
{
	int		final_len;
	char	must_expand;
	char	in_double_quotes;

	in_double_quotes = 0;
	must_expand = 1;
	final_len = 0;
	while (*line)
	{
		if (*line == '\"' && must_expand)
			in_double_quotes ^= 1;
		if (*line == '\'' && !in_double_quotes)
			must_expand ^= 1;
		if (*line == '$' && must_expand && !valid_environment_name(line))
		{
			final_len += get_expand_environment_len(line, env);
			line += get_environment_name_len(line) + 1;
		}
		else
		{
			final_len++;
			line++;
		}
	}
	return (final_len);
}

char	*remove_quotes(char *line, int flag)
{
	char	*ret_word;
	int		i;
	int		len;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	ret_word = NULL;
	i = 0;
	if (!(ft_strchr(line, '\'') || ft_strchr(line, '\"')))
		return (line);
	set_quote_with_null(line);
	while (!line[i] && i < len)
		i++;
	while (i < len)
	{
		ret_word = ft_strjoin(ret_word, &line[i], 1);
		i += ft_strlen(&line[i]) + 1;
		while (!line[i] && i < len)
			i++;
	}
	if (flag)
		free(line);
	return (ret_word);
}

void	set_quote_with_null(char *line)
{
	int	next_quotes;

	if (!line)
		return ;
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			next_quotes = get_next_quotes(line);
			*line = '\0';
			line += next_quotes;
			*line = '\0';
			line++;
		}
		else
			line++;
	}
	return ;
}

int	get_expand_environment_len(char *line, t_list *env)
{
	char				*env_word;
	int					len;
	extern unsigned int	g_exit_status;

	if (line && line[1] == '?')
	{
		if (g_exit_status == 0)
			return (1);
		else
			return (count_decimal(g_exit_status));
	}
	env_word = ft_substr(line, 1, get_environment_name_len(line));
	len = ft_strlen(get_environment_node(env, env_word));
	if (env_word)
		free(env_word);
	return (len);
}
