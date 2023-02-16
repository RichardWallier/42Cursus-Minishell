/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:16 by rwallier          #+#    #+#             */
/*   Updated: 2023/02/16 19:55:00 by rwallier         ###   ########.fr       */
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
	char	**environ;
}				t_data;

void	wrong_path(void);

int		run_commands(char **cmds, char **env);

int		matriz_len(char **matriz);

int		cd(char **path, t_data *data);

char	*ft_substr_free(char *str, unsigned int start, size_t len);

char	*ft_strjoin_free(char *s1, char const *s2);

#endif // !MINISHELL_H
