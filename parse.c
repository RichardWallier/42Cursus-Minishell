/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 00:02:39 by rwallier          #+#    #+#             */
/*   Updated: 2023/03/05 14:22:21 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	parse_env(t_data *data, char **environ)
{
	int	index;
	t_list	*temp;

	data->environ = ft_lstnew(ft_substr(environ[0], 0, ft_strlen(environ[0])));
	index = 1;
	while (environ[index])
	{
		temp = ft_lstnew(ft_substr(environ[index], 0, ft_strlen(environ[index])));
		ft_lstadd_back(&data->environ, temp);
		index++;
	}
	index = 0;
	return (1);
}
