/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 09:59:31 by rwallier          #+#    #+#             */
/*   Updated: 2023/04/02 18:09:53 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_linked_list	*ms_lstnew(void *content, int flag)
{
	t_linked_list	*result;

	result = (t_linked_list *)malloc(sizeof(t_linked_list));
	if (!result)
		return (NULL);
	result -> content = content;
	result->flag = flag;
	result -> next = NULL;
	return (result);
}
