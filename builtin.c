/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 18:29:46 by rwallier          #+#    #+#             */
/*   Updated: 2023/02/21 15:20:46 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	cd(char **cmd, t_data *data)
{
	if (matriz_len(cmd) > 2)
	{
		printf("cd: too many arguments\n");
		return (-1);
	}
	chdir(cmd[1]);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	return (1);
}
