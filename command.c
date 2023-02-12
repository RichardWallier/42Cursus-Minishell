/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:13 by rwallier          #+#    #+#             */
/*   Updated: 2023/02/12 18:52:19 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_arguments(char *argv)
{
	char	**args;
	char	**trimmed;
	int		index;

	args = ft_split2(argv, ' ');
	index = 0;
	while (args[index])
		index++;
	trimmed = (char **)malloc((index + 1) * sizeof(char *));
	index = 0;
	while (args[index])
	{
		trimmed[index] = ft_strtrim2(args[index], "'\"");
		free(args[index]);
		index++;
	}
	trimmed[index] = NULL;
	free(args);
	return (trimmed);
}

static char	*find_path(char *command, char **env)
{
	char	**path;
	char	*command_path;
	char	*temp;
	int		index;

	index = 0;
	while (env[index])
	{
		if (ft_strncmp("PATH=", env[index++], 5) == 0)
			path = ft_split(&env[index - 1][5], ':');
	}
	index = 0;
	while (path[index])
	{
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		temp = ft_strjoin(path[index], "/");
		free(path[index++]);
		command_path = ft_strjoin(temp, command);
		free(temp);
		if (access(command_path, F_OK | X_OK) == 0)
			return (command_path);
		free(command_path);
	}
	return ((char *)wrong_path);
}

int	run_commands(char *cmds, char **env)
{
	char	*path;
	char	**arguments;

	arguments = get_arguments(cmds);
	path = find_path(arguments[0], env);
	if (execve(path, arguments, env) == -1)
		wrong_path();
	return (0);
}
