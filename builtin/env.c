/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 01:43:24 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 01:43:32 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_builtiin(t_word *node)
{
	t_list	*env;

	if (node)
		env = node->env_lst;
	else
		env = NULL;
	while (env)
	{
		ft_putstr_fd(env->content, node->fd_out);
		ft_putchar_fd('\n', node->fd_out);
		env = env->next;
	}
	return (0);
}
