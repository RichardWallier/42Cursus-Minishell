/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 01:42:25 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 01:42:33 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	echo_builtin(t_word *node)
{
	uint8_t	newline;
	t_word	*head;

	head = node;
	newline = 1;
	if (node->next)
	{
		if (!ft_strncmp(node->next->word, "-n", 2))
		{
			newline = 0;
			node = node->next;
		}
		node = node->next;
		while (node && node->flag != MS_PIPE)
		{
			ft_putstr_fd(node->word, head->fd_out);
			node = node->next;
			if (node && node->flag != MS_PIPE)
				ft_putchar_fd(' ', head->fd_out);
		}
	}
	if (newline)
		ft_putchar_fd('\n', head->fd_out);
	return (0);
}
