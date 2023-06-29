/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 21:27:45 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:36:40 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_environment_node(t_list *env_node, char *env)
{
	char	*env_key;
	char	*env_line;

	env_line = NULL;
	while (env_node)
	{
		env_line = env_node->content;
		env_key = ft_substr(env_node->content,
				0, ft_strchr(env_line, '=') - env_line);
		if (!ft_strncmp(env_key, env, ft_strlen(env_key) + 1))
		{
			free(env_key);
			break ;
		}
		free(env_key);
		env_node = env_node->next;
	}
	if (!env_node)
		return (NULL);
	if (env_line)
		env_line = ft_strchr(env_line, '=') + 1;
	return (env_line);
}

char	**node_to_matrix(t_word *node)
{
	t_word				*aux;
	char				**mat;
	unsigned int		mat_positions;
	unsigned int		i;

	if (!node)
		return (NULL);
	aux = node;
	i = -1;
	mat_positions = 0;
	while (aux && aux->flag == MS_WORD)
	{
		aux = aux->next;
		mat_positions++;
	}
	mat = ft_calloc(mat_positions + 1, sizeof(char *));
	if (!mat)
		return (NULL);
	while (++i < mat_positions)
	{
		mat[i] = ft_strdup(node->word, 0);
		node = node->next;
	}
	return (mat);
}

int	parse_environment(t_data *data, char **environ)
{
	int		index;
	t_list	*temp;

	data->environ = ft_lstnew(ft_substr(environ[0], 0, ft_strlen(environ[0])));
	index = 1;
	while (environ[index])
	{
		temp = ft_lstnew(ft_substr(environ[index], 0,
					ft_strlen(environ[index])));
		ft_lstadd_back(&data->environ, temp);
		index++;
	}
	index = 0;
	return (1);
}
