/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelif.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 23:07:29 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:38:45 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstdelif(t_list **lst, char *ref)
{
	t_list	*temp;

	temp = *lst;
	if (!lst || !*lst)
		return (0);
	if (ft_strncmp(temp->content, ref, ft_strlen(ref)) == 0)
	{
		*lst = temp->next;
		free(temp->content);
		free(temp);
		ft_lstdelif(lst, ref);
	}
	else
		ft_lstdelif(&temp->next, ref);
	return (0);
}
