/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 21:36:32 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 21:39:25 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redirect_in(t_word *node)
{
	t_word	*head;

	head = node;
	while (head && head->flag != MS_WORD)
		head = head->next;
	while (head && node && node->flag != MS_PIPE)
	{
		if (node->flag == MS_REDIRECT_IN)
		{
			if (!access(node->next->word, F_OK | R_OK))
				head->fd_in = open(node->next->word, O_RDWR);
			else
			{
				printf("Error: No such file as '%s'\n", node->next->word);
				return (-1);
			}
		}
		else if (node->flag == MS_HEREDOC)
			head->fd_in = ms_heredoc(node);
		node = node->next;
	}
	return (0);
}

int	redirect_out(t_word *node)
{
	t_word	*head;

	head = node;
	while (head && head->flag != MS_WORD)
		head = head->next;
	while (head && node && node->flag != MS_PIPE)
	{
		if (node->flag == MS_REDIRECT_OUT || node->flag == MS_APPEND)
		{
			if (head->fd_out != STDOUT_FILENO)
				close(head->fd_out);
			if (node->flag == MS_REDIRECT_OUT)
				head->fd_out = open(node->next->word,
						O_WRONLY | O_CREAT | O_TRUNC, 0777);
			else
				head->fd_out = open(node->next->word,
						O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (head->fd_out == -1)
				return (-1);
		}
		node = node->next;
	}
	return (0);
}

int	do_redirections(t_word *node)
{
	if (!node)
		return (0);
	if (redirect_in(node) || redirect_out(node))
		return (-1);
	return (0);
}

int	has_pipe(t_word *node)
{
	while (node)
	{
		if (node->flag == MS_PIPE)
			return (1);
		node = node->next;
	}
	return (0);
}

void	exec_pipe(t_word *node, t_list **env_lst)
{
	uint16_t	builtin;

	builtin = is_builtin(node);
	node->pid = fork();
	if (node->pid != 0)
		return ;
	if (!builtin)
		exec_bin_pipe(node, *env_lst);
	else
		exec_builtin_pipe(node, env_lst, builtin);
}
