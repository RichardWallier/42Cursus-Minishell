/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 09:59:35 by rwallier          #+#    #+#             */
/*   Updated: 2023/04/02 18:10:23 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_lstsize(t_linked_list *lst)
{
	int	list_size;

	list_size = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		lst = lst -> next;
		list_size++;
	}
	return (list_size);
}
