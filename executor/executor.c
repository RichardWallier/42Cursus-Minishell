/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 21:35:07 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 09:51:31 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	executor(t_word **lst, t_list **env_lst, int flag)
{
	t_word		*node;
	t_word		**aux;

	node = *lst;
	open_pipe(node);
	while (node)
	{
		if (do_redirections(node) != 0)
			return (1);
		if (!flag)
			node = clean_sentence_redirections(lst, 1);
		else
			node = clean_sentence_redirections(aux, 0);
		if (has_pipe(*lst))
			exec_pipe(node, env_lst); 
		else
			exec_no_pipe(node, env_lst);
		close_sentence_fd(node);
		aux = get_next_command_address(node);
		node = get_next_command(node);
		flag++;
	}
	return (0);
}

t_word	*clean_sentence_redirections(t_word **lst, int flag)
{
	t_word	*ref;
	t_word	*aux;

	ref = *lst;
	ms_lst_remove_if(lst);
	if (flag && *lst && *lst != ref)
	{
		aux = *lst;
		while (aux)
		{
			aux->head = *lst;
			aux = aux->next;
		}
	}
	return (*lst);
}

void	close_sentence_fd(t_word *node)
{
	while (node && node->flag != MS_PIPE)
	{
		if (node->fd_in != STDIN_FILENO)
			close(node->fd_in);
		if (node->fd_out != STDOUT_FILENO)
			close(node->fd_out);
		node = node->next;
	}
	return ;
}

t_word	*get_next_command(t_word *node)
{
	while (node)
	{
		if (node->flag == MS_PIPE)
			return (node->next);
		node = node->next;
	}
	return (node);
}

t_word	**get_next_command_address(t_word *node)
{
	while (node && node->flag != MS_PIPE)
		node = node->next;
	if (node)
		return (&node->next);
	else
		return (NULL);
}
