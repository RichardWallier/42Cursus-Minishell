/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 01:40:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 09:48:29 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	export_util(char *arg, t_word *node);
static char	**get_environment_lst(t_list *env_node, char *env);

int	export_builtin(t_word *node)
{
	char	**av;
	int		i;

	av = node_to_matrix(node);
	if (!av)
		return (-1);
	i = 1;
	while (av[i])
	{
		export_util(av[i], node);
		i++;
	}
	free(av[0]);
	free(av);
	return (0);
}

static void	export_util(char *arg, t_word *node)
{
	char	**env_node;
	char	*env_name;

	if (arg[0] != '=' && ft_strchr(arg, '='))
	{
		env_name = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
		env_node = get_environment_lst(node->env_lst, env_name);
		if (env_node)
		{
			free(*env_node);
			*env_node = arg;
		}
		else
			ft_lstadd_back(&node->env_lst, ft_lstnew(arg));
		free(env_name);
	}
	else
		free(arg);
}

static char	**get_environment_lst(t_list *env_node, char *env)
{
	char	*env_key;
	char	*env_line;
	char	**env_line_ptr;

	env_line_ptr = NULL;
	if (!env_node || !env)
		return (NULL);
	while (env_node)
	{
		env_line = env_node->content;
		env_key = ft_substr(env_node->content,
				0, ft_strchr(env_line, '=') - env_line);
		if (!ft_strncmp(env_key, env, ft_strlen(env_key) + 1))
		{
			env_line_ptr = (char **)&env_node->content;
			free(env_key);
			break ;
		}
		free(env_key);
		env_node = env_node->next;
	}
	return (env_line_ptr);
}
