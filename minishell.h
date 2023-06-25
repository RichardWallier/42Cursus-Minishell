/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:16 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/25 15:28:21 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <locale.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"


typedef struct s_word
{
	char			*word;
	unsigned int	flag;
	int				fd_in;
	int				fd_out;
	int				ret;
	pid_t			pid;
	struct s_word	*head;
	struct s_list	*env_lst;
	struct s_word	*next;
}	t_word;
typedef struct s_data {
	t_word	*prompt;
	char	*bash;
	char	*pwd;
	t_list	*environ;
}				t_data;

typedef struct s_linked_list {
	void*					content;
	int						flag;
	struct s_linked_list	*next;
}				t_linked_list;

typedef struct s_pos
{
	int	i;
	int	j;
}	t_pos;

enum e_token
{
	MS_WORD = 1,
	MS_PIPE = 2,
	MS_REDIRECT_IN = 4,
	MS_REDIRECT_OUT = 8,
	MS_REDIRECT_FILE = 16,
	MS_APPEND = 32,
	MS_HEREDOC = 64,
};

enum e_builtins
{
	MS_ECHO = 256,
	MS_CD = 512,
	MS_PWD = 1024,
	MS_EXPORT = 2048,
	MS_UNSET = 4096,
	MS_ENV = 8192,
	MS_EXIT = 16384,
};

void	wrong_path(void);

int		run_commands(char **cmds, char **env);

int		matriz_len(char **matriz);

char	*ft_substr_free(char *str, unsigned int start, size_t len);

char	*ft_strjoin_free(char *s1, char const *s2);

int		free_split(char **split);

int		parse_env(t_data *data, char **environ);

int		is_only_whitespaces(char *str);

// Built in commands

int		env_builtin(t_data *data);

int		cd_builtin(char **path, t_data *data);

int		export_builtin(char **cmd, t_data *data);

int		unset_builtin(char **cmd, t_data *data);

#endif // !MINISHELL_H
