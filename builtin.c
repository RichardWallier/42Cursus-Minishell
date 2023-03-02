/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 18:29:46 by rwallier          #+#    #+#             */
/*   Updated: 2023/03/01 21:21:06 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	cd(char **cmd, t_data *data)
{
	int	index;

	index = -1;
	if (matriz_len(cmd) == 1)
	{
		while (data->environ[++index])
			if (ft_strncmp("HOME=", data->environ[index], 5) == 0)
				break;
		chdir(data->environ[index] + 5);
	}
	else
		chdir(cmd[1]);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	return (1);
}

int	env(t_data *data)
{
	int	index;

	index = -1;
	while (data->environ[++index])
		printf("%s\n", data->environ[index]);
	return (1);
}
