/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 22:37:14 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 22:38:30 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_bin_path(char **cmd, t_list *env)
{
	int					i;
	char				**path;
	char				*path_cmd;
	extern unsigned int	g_exit_status;

	if (!get_environment_node(env, "PATH"))
	{
		g_exit_status = 127;
		return (ft_putstr_fd("Ms: PATH unseted\n", STDERR_FILENO), 1);
	}
	i = -1;
	path = ft_split(get_environment_node(env, "PATH"), ':');
	while (path[++i])
	{
		path_cmd = ft_strjoin(path[i], *cmd, 0);
		if (access(path_cmd, F_OK | X_OK) == 0)
		{
			ft_free_matrix(path);
			free(*cmd);
			return (*cmd = path_cmd, 0);
		}
		free(path_cmd);
	}
	ft_putstr_fd("Ms: Command not found\n", STDERR_FILENO);
	return (ft_free_matrix(path), g_exit_status = 127, 1);
}
