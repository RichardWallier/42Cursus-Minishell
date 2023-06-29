/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 01:46:03 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:32:16 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	update_environment(t_list *env_lst, char *ref, char *newvalue);
static int	cd_with_params(t_word *node);
static int	cd_without_params(t_word *node);

int	cd_builtin(t_word *node)
{
	uint8_t	err;

	if (node->next && node->next->next && node->next->next->flag != MS_WORD)
		return (ft_putstr_fd("minishell: cd: Too many arguments\n",
				node->fd_out), 1);
	if (node->next && node->next->flag == MS_WORD)
		err = cd_with_params(node);
	else
		err = cd_without_params(node);
	return (err);
}

static int	cd_with_params(t_word *node)
{
	char	*pwd;

	if (node && node->next && node->next->flag == MS_WORD
		&& node->next->next && node->next->next->flag == MS_WORD)
		return (ft_putstr_fd("Ms: cd: too many arguments\n", STDERR_FILENO), 1);
	pwd = getcwd(NULL, 0);
	if (node->next->word[0] == '~')
		node->next->word = ft_strjoin(ft_strdup(
					get_environment_node(node->env_lst, "HOME"), 0),
				node->next->word + 1, 2);
	if (chdir(node->next->word) == -1)
	{
		ft_putstr_fd("Ms: cd: no such file or dir\n", STDERR_FILENO);
		return (free(pwd), 1);
	}
	if (update_environment(node->env_lst, "OLDPWD=", pwd) != 0)
		return (free(pwd), 1);
	free(pwd);
	pwd = getcwd(NULL, 0);
	if (update_environment(node->env_lst, "PWD=", pwd) != 0)
		return (1);
	free(pwd);
	return (0);
}

static int	cd_without_params(t_word *node)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (chdir(get_environment_node(node->env_lst, "HOME")) == -1)
	{
		ft_putstr_fd("minishell: cd: no such file or dir\n", node->fd_out);
		free(pwd);
		return (1);
	}
	if (update_environment(node->env_lst, "OLDPWD=", pwd) != 0)
		return (1);
	free(pwd);
	pwd = getcwd(NULL, 0);
	if (update_environment(node->env_lst, "PWD=", pwd) != 0)
		return (1);
	free(pwd);
	return (0);
}

static int	update_environment(t_list *env_lst, char *ref, char *newvalue)
{
	while (env_lst)
	{
		if (ft_strncmp(ref, env_lst->content, ft_strlen(ref)) == 0)
		{
			free(env_lst->content);
			env_lst->content = ft_strjoin(ref, newvalue, 0);
			if (!env_lst->content)
				return (1);
		}
		env_lst = env_lst->next;
	}
	return (0);
}
