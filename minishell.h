/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:16 by rwallier          #+#    #+#             */
/*   Updated: 2023/04/02 18:05:13 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <limits.h>

typedef struct s_data {
	char	**prompt;
	char	*bash;
	char	*pwd;
	char	**environ_pointer;
	t_list	*environ;
}				t_data;

typedef struct s_linked_list {
	void*					content;
	int						flag;
	struct s_linked_list	*next;
}				t_linked_list;

void	wrong_path(void);

int		run_commands(char **cmds, char **env);

int		matriz_len(char **matriz);

char	*ft_substr_free(char *str, unsigned int start, size_t len);

char	*ft_strjoin_free(char *s1, char const *s2);

int	free_split(char **split);

int		parse_env(t_data *data, char **environ);

// Built in commands

int		env_builtin(t_data *data);

int		cd_builtin(char **path, t_data *data);

int		export_builtin(char **cmd, t_data *data);

int		unset_builtin(char **cmd, t_data *data);

int		exit_builtin(t_data *data);

#endif // !MINISHELL_H
