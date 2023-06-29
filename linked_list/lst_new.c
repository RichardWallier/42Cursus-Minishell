/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:50:18 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 12:50:26 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_word	*ms_lstnew(void *word)
{
	t_word	*new;

	new = (t_word *)malloc(sizeof(t_word));
	if (!new)
		return (NULL);
	new->word = word;
	new->flag = 0;
	new->pid = 0;
	new->fd_in = 0;
	new->fd_out = 1;
	new->ret = -1;
	new->next = NULL;
	return (new);
}
