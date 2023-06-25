/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:16 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/25 16:09:36 by rwallier         ###   ########.fr       */
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

int		exit_error_code(t_word **lst);

void	exec_builtin_pipe(t_word *node, t_list **env_lst, uint16_t builtin);

static int	check_bin_current_dir(char **cmd);

static int	check_bin_path(char **cmd, t_list *env);

int	is_builtin(t_word *node);

void	delete_environment(t_list **node, char *ref);
char	*get_environment_node(t_list *env_node, char *env);
int	update_environment(t_list *env_lst, char *ref, char *newvalue);

// Ms lst functions

void	ms_lstclear(t_word **lst, int flag);

char	**lst_to_matrix(t_word *node);

// Built in commands

int		echo_builtin(t_word *node);

int		cd_builtin(t_word *node);
int		cd_with_params(t_word *node);
int		cd_without_params(t_word *node);

int		pwd_builtin(t_word *node);

int		export_builtin(t_word *node);
void	export_util(char *arg, t_word *node);

int		env_builtiin(t_word *node);

int		unset_builtin(t_word *node, t_list **env);

void	exit_builtin(t_word **word, t_list **env_lst);

int		exit_builtin_pipe(t_word **word, t_list **env_lst);

#endif // !MINISHELL_H
