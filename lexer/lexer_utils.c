/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 13:04:58 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 21:16:39 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_flag_word(char *word, int last_flag)
{
	if (!word)
		return (-1);
	if (ft_strncmp(word, "|", 2) == 0)
		return (MS_PIPE);
	else if (ft_strncmp(word, "<<", 3) == 0)
		return (MS_HEREDOC);
	else if (ft_strncmp(word, ">>", 3) == 0)
		return (MS_APPEND);
	else if (ft_strncmp(word, "<", 2) == 0)
		return (MS_REDIRECT_IN);
	else if (ft_strncmp(word, ">", 2) == 0)
		return (MS_REDIRECT_OUT);
	else if (last_flag == MS_REDIRECT_OUT  || last_flag == MS_REDIRECT_IN
		|| last_flag == MS_HEREDOC || last_flag == MS_APPEND)
		return (MS_REDIRECT_FILE);
	return (MS_WORD);
}

char	*expand_environment(char *line, t_list *env)
{
	char	*ret_line;

	ret_line = ft_calloc(get_environment_len_after_expand(line, env) + 1,
			sizeof(char));
	if (ret_line)
		expand_environment_util(line, ret_line, env);
	free(line);
	return (ret_line);
}

void	expand_environment_util(char *line, char *ret_line, t_list *env)
{
	t_pos	aux;
	char	must_expand;
	char	in_double_quotes;

	aux = (t_pos){0};
	must_expand = 1;
	in_double_quotes = 1;
	while (line[aux.i])
	{
		if (line[aux.i] == '\"' && must_expand)
			in_double_quotes ^= 1;
		if (line[aux.i] == '\'' && in_double_quotes)
			must_expand ^= 1;
		if (line[aux.i] == '$' && must_expand
			&& !valid_environment_name(&line[aux.i]))
			expand_environment_utils_check_line(line, ret_line, &aux, env);
		else
		{
			ret_line[aux.j] = line[aux.i];
			aux.i++;
			aux.j++;
		}
	}
	return ;
}

int	analyze_pipe_syntax(t_word *word_lst)
{
	t_word	*node;
	int		last_flag;

	if (!word_lst)
		return (-1);
	node = word_lst;
	last_flag = MS_PIPE;
	while (node)
	{
		if ((last_flag == MS_PIPE && node->flag == MS_PIPE)
			|| (!node->next && node->flag == MS_PIPE))
		{
			printf("Syntax error: Incorrect use of pipes\n");
			return (-1);
		}
		last_flag = node->flag;
		node = node->next;
	}
	return (0);
}

int	analyze_redirect_syntax(t_word *word_lst)
{
	t_word	*node;

	if (!word_lst)
		return (-1);
	node = word_lst;
	while (node)
	{
		if (node->flag == MS_REDIRECT_IN || node->flag == MS_REDIRECT_OUT
			|| node->flag == MS_HEREDOC || node->flag == MS_APPEND)
		{
			if (!node->next || node->next->flag != MS_REDIRECT_FILE)
			{
				printf("Syntax error: error on '%s'\n", node->word);
				return (-1);
			}
		}
		node = node->next;
	}
	return (0);
}
