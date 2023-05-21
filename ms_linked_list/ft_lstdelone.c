/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 09:59:22 by rwallier          #+#    #+#             */
/*   Updated: 2023/04/02 18:07:57 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_lstdelone(t_linked_list *lst, void (*del)(void *))
{
	if (!lst || ! del)
		return ;
	(del)(lst-> content);
	free(lst);
}
