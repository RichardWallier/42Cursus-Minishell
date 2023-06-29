/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 22:35:41 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 09:50:42 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_pipe(int *fd);

char	**env_to_matrix(t_list *node)
{
	char	**mat;
	int		mat_size;
	int		i;

	mat_size = ft_lstsize(node);
	if (!mat_size)
		return (NULL);
	mat = ft_calloc(mat_size + 1, sizeof(char *));
	if (!mat)
		return (NULL);
	mat[mat_size] = NULL;
	i = 0;
	while (node)
	{
		mat[i] = ft_strdup(node->content, 0);
		if (!mat[i])
		{
			ft_free_matrix(mat);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (mat);
}

int	ms_heredoc(t_word *node)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	line = NULL;
	while (42)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(node->next->word, line, ft_strlen(line) + 1))
			break ;
		else if (line)
		{
			write (fd[1], line, ft_strlen(line));
			write (fd[1], "\n", 1);
			free(line);
		}
	}
	if (line)
		free(line);
	close (fd[1]);
	return (fd[0]);
}

int	open_pipe(t_word *node)
{
	int		fd[2];
	t_word	*cmd;

	while (node)
	{
		while (node && node->flag != MS_PIPE && node->flag != MS_WORD)
			node = node->next;
		cmd = node;
		while (node && node->flag != MS_PIPE)
			node = node->next;
		if (!node)
			return (0);
		if (pipe(fd) == -1)
			return (1);
		cmd->fd_out = fd[1];
		node = node->next;
		cmd = node;
		while (cmd && cmd->flag != MS_PIPE && cmd->flag != MS_WORD)
			cmd = cmd->next;
		if (cmd)
			cmd->fd_in = fd[0];
		else
			close_pipe(fd);
	}
	return (0);
}

static void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
	return ;
}
