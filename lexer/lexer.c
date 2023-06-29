/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 13:02:40 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:38:54 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	lexical_analyzer(t_word **word_lst)
{
	if (!word_lst)
		return (1);
	if (lexx(word_lst))
		return (1);
	if (syntax_analyze(*word_lst))
		return (1);
	if (analyze_command_syntax(*word_lst))
		return (1);
	return (0);
}

int	lexx(t_word **lst)
{
	t_word	*node;
	int		last_flag;

	if (!lst || !*lst)
		return (1);
	node = *lst;
	last_flag = 0;
	while (node)
	{
		if (node->word)
		{
			node->flag = get_flag_word(node->word, last_flag);
			node->word = expand_environment(node->word, node->env_lst);
			node->word = remove_quotes(node->word, 1);
			last_flag = node->flag;
		}
		node = node->next;
	}
	lst_remove_empty_word(lst);
	return (0);
}

int	syntax_analyze(t_word *word_lst)
{
	if (!word_lst)
		return (-1);
	else if (analyze_pipe_syntax(word_lst))
		return (-1);
	else if (analyze_redirect_syntax(word_lst))
		return (-1);
	return (0);
}

int	analyze_command_syntax(t_word *word_lst)
{
	while (word_lst)
	{
		if (get_command_count(word_lst) == 0)
		{
			ft_putstr_fd("Syntax error: sentence w/no command\n", STDERR_FILENO);
			return (1);
		}
		while (word_lst && word_lst->flag != MS_PIPE)
			word_lst = word_lst->next;
		if (word_lst)
			word_lst = word_lst->next;
	}
	return (0);
}
