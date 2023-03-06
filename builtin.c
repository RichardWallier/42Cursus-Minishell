/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 18:29:46 by rwallier          #+#    #+#             */
/*   Updated: 2023/03/05 23:08:13 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	cd_builtin(char **cmd, t_data *data)
{
	if (matriz_len(cmd) == 1)
	{
		while (data->environ->next)
			if (ft_strncmp("HOME=", data->environ->content, 5) == 0)
				break;
		chdir(data->environ->content + 5);
	}
	else
		chdir(cmd[1]);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	return (1);
}

int	env_builtin(t_data *data)
{
	t_list	*temp;

	temp = data->environ;
	while (temp)
	{
		printf("%s\n", (char *)temp->content);
		temp = temp->next;
	}
	return (1);
}

int	export_builtin(char **cmd, t_data *data)
{
	ft_lstadd_back(&data->environ, ft_lstnew(cmd[1]));
	return (1);
}


int	unset_builtin(char **cmd, t_data *data)
{
	ft_lstdelif(&data->environ, cmd[1]);
	return (1);
}

