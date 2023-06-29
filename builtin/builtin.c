/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 22:50:08 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:32:49 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	exit_error_code(t_word **lst);

void	exec_builtin(t_word *node, t_list **env_lst, uint16_t builtin)
{
	if (builtin == MS_ECHO)
		node->ret = echo_builtin(node);
	else if (builtin == MS_CD)
		node->ret = cd_builtin(node);
	else if (builtin == MS_PWD)
		node->ret = pwd_builtin(node);
	else if (builtin == MS_EXPORT)
		node->ret = export_builtin(node);
	else if (builtin == MS_ENV)
		node->ret = env_builtiin(node);
	else if (builtin == MS_UNSET)
		node->ret = unset_builtin(node, env_lst);
	else if (builtin == MS_EXIT)
		exit_builtin(&node->head, &node->env_lst);
	return ;
}

void	exec_builtin_pipe(t_word *node, t_list **env_lst, uint16_t builtin)
{
	if (builtin == MS_ECHO)
		exit(echo_builtin(node));
	else if (builtin == MS_CD)
		exit(cd_builtin(node));
	else if (builtin == MS_PWD)
		exit(pwd_builtin(node));
	else if (builtin == MS_EXPORT)
		exit(export_builtin(node));
	else if (builtin == MS_ENV)
		exit(env_builtiin(node));
	else if (builtin == MS_UNSET)
		exit(unset_builtin(node, env_lst));
	else if (builtin == MS_EXIT)
		exit(exit_builtin_pipe(&node, &node->env_lst));
	return ;
}

int	exit_builtin_pipe(t_word **word, t_list **env_lst)
{
	int	ret;

	ret = exit_error_code(word);
	ft_lstclear(env_lst, free);
	ms_lstclear(word, 0);
	rl_clear_history();
	return (ret);
}

void	exit_builtin(t_word **word, t_list **env_lst)
{
	unsigned char	ret;

	ret = exit_error_code(word);
	ft_lstclear(env_lst, free);
	ms_lstclear(word, 0);
	rl_clear_history();
	exit(ret);
	return ;
}

static int	exit_error_code(t_word **lst)
{
	t_word				*node;

	if (!lst || !*lst)
		return (0);
	node = *lst;
	if (node->next && node->next->flag == MS_WORD
		&& node->next->next && node->next->next->flag == MS_WORD)
	{
		ft_putstr_fd("Minishell: Exit: Too many args\n", STDERR_FILENO);
		return (1);
	}
	else if (node->next && node->next->flag == MS_WORD)
	{
		if (str_is_num(node->next->word) == 0)
			return (ft_atoi(node->next->word));
		else
		{
			ft_putstr_fd("Minishell: Exit: Invalid number\n", STDERR_FILENO);
			return (255);
		}
	}
	return (0);
}
