/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 21:40:43 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/28 22:30:25 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(t_word *node)
{
	if (!node)
		return (0);
	if (ft_strncmp(node->word, "echo", 5) == 0)
		return (MS_ECHO);
	else if (ft_strncmp(node->word, "cd", 3) == 0)
		return (MS_CD);
	else if (ft_strncmp(node->word, "pwd", 4) == 0)
		return (MS_PWD);
	else if (ft_strncmp(node->word, "export", 7) == 0)
		return (MS_EXPORT);
	else if (ft_strncmp(node->word, "unset", 6) == 0)
		return (MS_UNSET);
	else if (ft_strncmp(node->word, "exit", 5) == 0)
		return (MS_EXIT);
	else if (ft_strncmp(node->word, "env", 4) == 0)
		return (MS_ENV);
	return (0);
}

void	exec_bin_pipe(t_word *node, t_list *env_lst)
{
	char		*cmd;
	char		**mat;
	char		**env_mat;

	if (!node)
		return ;
	cmd = check_bin(node->word, env_lst);
	if (!cmd)
		exit(127);
	signal(SIGQUIT, SIG_DFL);
	if (node->fd_out != STDOUT_FILENO)
		dup2(node->fd_out, STDOUT_FILENO);
	if (node->fd_in != STDIN_FILENO)
		dup2(node->fd_in, STDIN_FILENO);
	close_all_fd(node->head);
	mat = node_to_matrix(node);
	env_mat = ft_lsttochrmat(node->env_lst);
	execve(cmd, mat, env_mat);
	return ;
}

void	close_all_fd(t_word *node)
{
	while (node)
	{
		if (node->fd_in != STDIN_FILENO)
			close(node->fd_in);
		else if (node->fd_out != STDOUT_FILENO)
			close(node->fd_out);
		node = node->next;
	}
	return ;
}

void	exec_no_pipe(t_word *node, t_list **env_lst)
{
	if (!is_builtin(node))
		exec_bin(node, *env_lst);
	else
		exec_builtin(node, env_lst, is_builtin(node));
}

int	exec_bin(t_word *node, t_list *env_lst)
{
	char		*cmd;
	char		**cmd_mat;
	char		**env_mat;

	if (!node)
		return (0);
	cmd = check_bin(node->word, env_lst);
	if (!cmd)
		return (-1);
	node->pid = fork();
	if (node->pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		if (node->fd_out != STDOUT_FILENO)
			dup2(node->fd_out, STDOUT_FILENO);
		if (node->fd_in != STDIN_FILENO)
			dup2(node->fd_in, STDIN_FILENO);
		close_all_fd(node->head);
		cmd_mat = node_to_matrix(node);
		env_mat = env_to_matrix(node->env_lst);
		execve(cmd, cmd_mat, env_mat);
		return (0);
	}
	free(cmd);
	return (0);
}
