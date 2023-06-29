/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_remove_empty_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:52:58 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 12:53:06 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lst_remove_empty_word(t_word **head)
{
	t_word	*node;

	if (!head || !(*head))
		return ;
	node = *head;
	if (!node->word || !node->word[0])
	{
		*head = node->next;
		if (node->word)
			free(node->word);
		free(node);
		ms_lst_remove_if(head);
	}
	node = *head;
	if (!head || !*head)
		return ;
	lst_remove_empty_word(&node->next);
	return ;
}