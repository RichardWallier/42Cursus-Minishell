/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:34:57 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parser(char *line, t_data *data)
{
	int	error;

	error = 0;
	if (check_quote(line))
	{
		printf("Syntax error: There is an unclosed quote\n");
		free(line);
		return (-1);
	}
	data->prompt = parse_prompt(line, data->environ);
	error = lexical_analyzer(&data->prompt);
	free(line);
	return (error);
}

t_word	*parse_prompt(char *line, t_list *env_lst)
{
	int		word_len;
	t_word	*new_word;
	t_word	*word_lst;

	if (!line)
		return (NULL);
	word_lst = NULL;
	while (ft_isspace(*line))
		line++;
	while (*line)
	{
		word_len = get_word_len(line);
		new_word = ms_lstnew(ft_substr(line, 0, word_len));
		new_word->env_lst = env_lst;
		ms_lstadd_back(&word_lst, new_word);
		line += word_len;
		while (ft_isspace(*line))
			line++;
	}
	set_head(word_lst);
	return (word_lst);
}
