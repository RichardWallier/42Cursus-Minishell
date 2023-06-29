/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 22:47:05 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 22:47:06 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_cmds(t_word *node)
{
	t_word				*aux;
	extern unsigned int	g_exit_status;

	if (!node)
		return ;
	aux = node;
	while (node)
	{
		if (node->pid != 0)
			waitpid(node->pid, &node->ret, 0);
		node = node->next;
	}
	node = aux;
	while (node)
	{
		if (node->ret >= 0)
			g_exit_status = WEXITSTATUS(node->ret);
		node = node->next;
	}
	return ;
}
