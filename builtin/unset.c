/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 01:38:38 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 09:45:35 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	delete_environment(t_list **node, char *ref);

int	unset_builtin(t_word *node, t_list **env)
{
	t_word	*aux;
	char	**av;
	int		i;

	av = node_to_matrix(node);
	if (!av)
		return (-1);
	i = 1;
	while (av[i])
	{
		delete_environment(env, av[i]);
		i++;
	}
	if (*env != node->head->env_lst)
	{
		aux = node->head;
		while (aux)
		{
			aux->env_lst = *env;
			aux = aux->next;
		}
	}
	ft_free_matrix(av);
	return (0);
}

static void	delete_environment(t_list **node, char *ref)
{
	char	*env_key;
	t_list	*aux;

	if (!node || !*node)
		return ;
	aux = *node;
	env_key = ft_substr(aux->content, 0,
			ft_strchr(aux->content, '=') - (char *) aux->content);
	if (!ft_strncmp(ref, env_key, ft_strlen(env_key) + 1))
	{
		*node = aux->next;
		free(aux->content);
		free(aux);
		free(env_key);
		return ;
	}
	else
		delete_environment(&aux->next, ref);
	free(env_key);
	return ;
}
