/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/02/12 18:56:52 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char		*prompt;
	char		*bash;
	char		path[PATH_MAX];
	int			pid;
	extern char	**environ;

	while (42)
	{
		getcwd(path, sizeof(path));
		bash = ft_strjoin(path, " $ ");
		prompt = readline(bash);
		pid = fork();
		if (pid == 0)
			run_commands(prompt, environ);
		waitpid(pid, NULL, 0);
	}
	return (0);
}
