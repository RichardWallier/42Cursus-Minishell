/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:16 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 09:50:04 by rwallier         ###   ########.fr       */
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

//signal
void	set_sighandle(void);
void	sigint_handle(int signal);

// Parser 
int		parser(char *line, t_data *data);
int		check_quote(char *line);
int		get_next_quotes(char *line);
t_word	*parse_prompt(char *line, t_list *env_lst);

//utils
int		get_word_len(char *line);
int		is_meta_char(char *c);
int		get_next_quotes(char *line);
int		check_quote(char *line);
char	*get_environment_node(t_list *env_node, char *env);
char	**node_to_matrix(t_word *node);
int		str_is_num(char *str);
int		parse_environment(t_data *data, char **environ);
void	wait_cmds(t_word *node);

//linked list
void	ms_lstclear(t_word **lst, int flag);
void	set_head(t_word *head);
void	ms_lst_remove_if(t_word **head);
void	lst_remove_empty_word(t_word **head);
t_word	*ms_lstnew(void *word);
void	ms_lstclear(t_word **lst, int flag);
void	ms_lstadd_back(t_word **lst, t_word *new);

//lexxer
int		lexical_analyzer(t_word **word_lst);
int		lexx(t_word **lst);
int		syntax_analyze(t_word *word_lst);
int		analyze_command_syntax(t_word *word_lst);

//lexer utils
char	*expand_environment(char *line, t_list *env);
void	expand_environment_util(char *line, char *ret_line, t_list *env);
int		get_flag_word(char *word, int last_flag);
int		analyze_pipe_syntax(t_word *word_lst);
int		analyze_redirect_syntax(t_word *word_lst);
int		get_command_count(t_word *node);
int		get_environment_len_after_expand(char *line, t_list *env);
char	*remove_quotes(char *line, int flag);
void	set_quote_with_null(char *line);
int		get_expand_environment_len(char *line, t_list *env);
void	expand_environment_utils_check_line(char *line, char *ret_line, t_pos *aux, t_list *env);
int		get_environment_name_len(char *line);
int		valid_environment_name(char *line);
int		check_bin_path(char **cmd, t_list *env);

//executor
int	open_pipe(t_word *node);
int	executor(t_word **lst, t_list **env_lst, int flag);
t_word	*clean_sentence_redirections(t_word **lst, int flag);
void	close_sentence_fd(t_word *node);
t_word	*get_next_command(t_word *node);
t_word	**get_next_command_address(t_word *node);

//executor utils
int		redirect_in(t_word *node);
int		redirect_out(t_word *node);
int		do_redirections(t_word *node);
int		has_pipe(t_word *node);
void	exec_pipe(t_word *node, t_list **env_lst);
int		is_builtin(t_word *node);
void	exec_bin_pipe(t_word *node, t_list *env_lst);
char	*check_bin(char *cmd, t_list *env);
int		check_bin_current_dir(char **cmd);
int		open_pipe(t_word *node);
void	exec_no_pipe(t_word *node, t_list **env_lst);
int		exec_bin(t_word *node, t_list *env_lst);
void	close_all_fd(t_word *node);
char	**env_to_matrix(t_list *node);
int		ms_heredoc(t_word *node);

//builtins
void	exec_builtin_pipe(t_word *node, t_list **env_lst, uint16_t builtin);
int		exit_builtin_pipe(t_word **word, t_list **env_lst);
void	exit_builtin(t_word **word, t_list **env_lst);
void	exec_builtin(t_word *node, t_list **env_lst, uint16_t builtin);

//builtins functions
int	cd_builtin(t_word *node);
int	env_builtiin(t_word *node);
int	unset_builtin(t_word *node, t_list **env);
int	pwd_builtin(t_word *node);
int	export_builtin(t_word *node);
int	echo_builtin(t_word *node);
int	env_builtiin(t_word *node);

// Ms lst functions
void	ms_lstclear(t_word **lst, int flag);
void	ms_lstadd_back(t_word **lst, t_word *new);
void	lst_remove_empty_word(t_word **head);
t_word	*ms_lstnew(void *word);
void	ms_lst_remove_if(t_word **head);
void	set_head(t_word *head);


#endif // !MINISHELL_H
