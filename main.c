/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/03/01 21:23:38 by rwallier         ###   ########.fr       */
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
	while (42)
	{
		data.bash = ft_strjoin(data.pwd, "$ ");
		temp = readline(data.bash);
		data.prompt = ft_split2(temp, ' ');
		free(data.bash);
		if (ft_strncmp(*data.prompt, "cd", ft_strlen(*data.prompt)) == 0)
			cd(data.prompt, &data);
		else if (ft_strncmp(*data.prompt, "env", ft_strlen(*data.prompt)) == 0)
			env(&data);
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
