/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:55:51 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 01:50:53 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	g_exit_status;

int	main(void)
{
	t_data		data;
	int			pid;
	char		*line;
	extern char	**environ;

	parse_environment(&data, environ);
	set_sighandle();
	while (42)
	{
		data.pwd = get_environment_node(data.environ, "PWD");
		data.bash = ft_strjoin(data.pwd, "$ ", 0);
		line = readline(data.bash);
		free(data.bash);
		if (!line)
			exit_builtin(&data.prompt, &data.environ);
		if (line[0])
			add_history(line);
		if (parser(line, &data)) 
		{
			ms_lstclear(&data.prompt, 1);
			continue ;	
		}
		executor(&data.prompt, &data.environ, 0);
		wait_cmds(data.prompt);
		ms_lstclear(&data.prompt, 1);
	}
	return (0);
}

