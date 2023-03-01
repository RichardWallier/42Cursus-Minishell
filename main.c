/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/03/01 20:40:32 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_data		data;
	int			pid;
	char		*temp;
	extern char	**environ;

	data.pwd = getcwd(NULL, 0);
	data.environ = environ;
	for (int i = 0; environ[i] != NULL; i++)
		printf("environ[%i]: %s\n", i, data.environ[i]);
	while (42)
	{
		data.bash = ft_strjoin(data.pwd, "$ ");
		temp = readline(data.bash);
		data.prompt = ft_split2(temp, ' ');
		free(data.bash);
		if (ft_strncmp(*data.prompt, "cd", ft_strlen(*data.prompt)) == 0)
			cd(data.prompt, &data);
		else
		{
			pid = fork();
			if (pid == 0)
				run_commands(data.prompt, environ);
		}
		waitpid(pid, NULL, 0);
	}
	return (0);
}
