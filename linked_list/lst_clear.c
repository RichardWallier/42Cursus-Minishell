/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstclear.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:49:22 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 12:49:30 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_lstclear(t_word **lst, int flag)
{
	t_word	*aux;
	t_word	*node;

	if (!lst || !(*lst))
		return ;
	node = *lst;
	while (node)
	{
		aux = node->next;
		free(node->word);
		free(node);
		node = aux;
	}
	if (flag)
		*lst = NULL;
	return ;
}
