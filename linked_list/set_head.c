/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_head.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:51:49 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 12:51:58 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_head(t_word *head)
{
	t_word	*aux;

	if (!head)
		return ;
	aux = head;
	while (aux)
	{
		aux->head = head;
		aux = aux->next;
	}
	return ;
}
