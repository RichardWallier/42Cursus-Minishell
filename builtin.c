/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 18:29:46 by rwallier          #+#    #+#             */
/*   Updated: 2023/02/16 19:54:40 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	cd(char **cmd, t_data *data)
{
	int		index;
	char	*temp;

	if (matriz_len(cmd) > 2)
	{
		printf("cd: too many arguments\n");
		return (-1);
	}
	if (ft_strncmp(cmd[1], "..", ft_strlen(cmd[1])) == 0)
	{
		index = ft_strlen(data->pwd) - 1;
		while (index && data->pwd[index] != '/')
			index--;
		data->pwd = ft_substr(data->pwd, 0, index);
	}
	else
	{
		temp = ft_strjoin_free(data->pwd, "/");
		data->pwd = ft_strjoin_free(temp, cmd[1]);
		index = ft_strlen(data->pwd) - 1;
		if (index && data->pwd[index] == '/')
			data->pwd = ft_substr(data->pwd, 0, index);
	}
	chdir(data->pwd);
	return (1);
}
