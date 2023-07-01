/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:55:51 by rwallier          #+#    #+#             */
/*   Updated: 2023/07/01 16:21:10 by wcaetano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init(t_data *data);

unsigned int	g_exit_status;

int	main(void)
{
	t_data		data;
	char		*line;

	init(&data);
	while (42)
	{
		data.bash = ft_strjoin(get_environment_node(data.environ, "PWD"),
				"$ ", 0);
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

static void	init(t_data *data)
{
	extern char	**environ;

	parse_environment(data, environ);
	set_sighandle();
}
