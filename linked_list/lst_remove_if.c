/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_remove_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:52:18 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:38:01 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_lst_remove_if(t_word **head)
{
	t_word	*node;

	if (!head || !*head || (*head)->flag == MS_PIPE)
		return ;
	node = *head;
	if (node->flag != MS_WORD)
	{
		*head = node->next;
		free(node->word);
		free(node);
		ms_lst_remove_if(head);
	}
	node = *head;
	if (!head || !*head || (*head)->flag == MS_PIPE)
		return ;
	ms_lst_remove_if(&node->next);
	return ;
}
