/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/25 14:19:27 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote(char *line);
int	get_next_quotes(char *line);
t_word	*ms_lstnew(void *word);
void	ms_lstadd_back(t_word **lst, t_word *new);
t_word	*parse_prompt(char *line, t_list *env_lst);
int	parser(char *line, t_data *data);
int	is_meta_char(char *c);
int	has_pipe(t_word *node);
int	ms_pipe(t_word *node);
void	close_pipe(int *fd);
t_word	*clean_sentence_redirections(t_word **lst, int flag);
void	exec_no_pipe(t_word *node, t_list **env_lst);
int	exec_bin(t_word *node, t_list *env_lst);
void	exec_builtin(t_word *node, t_list **env_lst, uint16_t builtin);
char	*check_bin(char *cmd, t_list *env);
void	free_matrix(char **mat);
void	close_all_fd(t_word *node);
char	**node_to_matrix(t_word *node);
char	**env_to_matrix(t_list *node);


int	main(void)
{
	t_data		data;
	int			pid;
	char		*line;
	extern char	**environ;

	data.pwd = getcwd(NULL, 0);
	parse_env(&data, environ);

	while (42)
	{
		data.bash = ft_strjoin(data.pwd, "$ ", 0);
		line = readline(data.bash);
		free(data.bash);
		if (!line)
			exit_builtin(&data.prompt, &data.environ);
		if (!is_only_whitespaces(line))
			add_history(line);
		if (parser(&line, &data)) 
		{
			ms_lstclear(&data.prompt, 1);
			continue ;	
		}
		ms_executor(&data.prompt, &data.environ, 0);
		ms_wait_cmds(data.prompt);
		ms_lstclear(&data.prompt, 1);
	}
	return (0);
}

void	ms_wait_cmds(t_word *node)
{
	t_word				*aux;
	extern unsigned int	g_exit_status;

	if (!node)
		return ;
	aux = node;
	while (node)
	{
		if (node->pid != 0)
			waitpid(node->pid, &node->ret, 0);
		node = node->next;
	}
	node = aux;
	while (node)
	{
		if (node->ret >= 0)
			g_exit_status = WEXITSTATUS(node->ret);
		node = node->next;
	}
	return ;
}

int	ms_executor(t_word **lst, t_list **env_lst, int flag)
{
	t_word		*node;
	t_word		**aux;

	node = *lst;
	ms_pipe(node);
	while (node)
	{
		if (ms_do_redirections(node) != 0)
			return (1);
		if (!flag)
			node = clean_sentence_redirections(lst, 1);
		else
			node = clean_sentence_redirections(aux, 0);
		if (!has_pipe(*lst))
			exec_no_pipe(node, env_lst); 
		else
			exec_pipe(node, env_lst);
		ms_close_sentence_fd(node);
		aux = ms_get_next_cmd_addr(node);
		node = ms_get_next_command(node);
		flag++;
	}
	return (0);
}

t_word	*ms_get_next_command(t_word *node)
{
	while (node)
	{
		if (node->flag == MS_PIPE)
			return (node->next);
		node = node->next;
	}
	return (node);
}

t_word	**ms_get_next_cmd_addr(t_word *node)
{
	while (node && node->flag != MS_PIPE)
		node = node->next;
	if (node)
		return (&node->next);
	else
		return (NULL);
}

void	ms_close_sentence_fd(t_word *node)
{
	while (node && node->flag != MS_PIPE)
	{
		if (node->fd_in != STDIN_FILENO)
			close(node->fd_in);
		if (node->fd_out != STDOUT_FILENO)
			close(node->fd_out);
		node = node->next;
	}
	return ;
}

void	exec_pipe(t_word *node, t_list **env_lst)
{
	uint16_t	builtin;

	builtin = is_builtin(node);
	node->pid = fork();
	if (node->pid != 0)
		return ;
	if (!builtin)
		exec_bin_pipe(node, *env_lst);
	else
		exec_builtin_pipe(node, env_lst, builtin);
}

void	exec_builtin_pipe(t_word *node, t_list **env_lst, uint16_t builtin)
{
	if (builtin == MS_ECHO)
		exit(ms_echo(node));
	else if (builtin == MS_CD)
		exit(ms_cd(node));
	else if (builtin == MS_PWD)
		exit(ms_pwd(node));
	else if (builtin == MS_EXPORT)
		exit(ms_export(node));
	else if (builtin == MS_ENV)
		exit(ms_env(node));
	else if (builtin == MS_UNSET)
		exit(ms_unset(node, env_lst));
	else if (builtin == MS_EXIT)
		exit(ms_exit_pipe(&node, &node->env_lst));
	return ;
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
	env_mat = lst_to_matrix(node->env_lst);
	execve(cmd, mat, env_mat);
	return ;
}

char	*check_bin(char *cmd, t_list *env)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd, 0));
		else
		{
			return (ft_putstr_fd("Ms: cmd not found\n", STDERR_FILENO), NULL);
		}
	}
	cmd = ft_strjoin("/", cmd, 0);
	if (ms_check_bin_current_dir(&cmd) == 0)
		return (cmd);
	else if (ms_check_bin_path(&cmd, env) == 0)
		return (cmd);
	free(cmd);
	return (NULL);
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
		cmd_mat = lst_to_matrix(node);
		env_mat = env_to_matrix(node->env_lst);
		execve(cmd, cmd_mat, env_mat);
		return (0);
	}
	free(cmd);
	return (0);
}

void	exec_builtin(t_word *node, t_list **env_lst, uint16_t builtin)
{
	if (builtin == MS_ECHO)
		node->ret = ms_echo(node);
	else if (builtin == MS_CD)
		node->ret = ms_cd(node);
	else if (builtin == MS_PWD)
		node->ret = ms_pwd(node);
	else if (builtin == MS_EXPORT)
		node->ret = ms_export(node);
	else if (builtin == MS_ENV)
		node->ret = ms_env(node);
	else if (builtin == MS_UNSET)
		node->ret = ms_unset(node, env_lst);
	else if (builtin == MS_EXIT)
		exit_builtin(&node->head, &node->env_lst);
	return ;
}

int	ms_unset(t_word *node, t_list **env)
{
	t_word	*aux;
	char	**av;
	int		i;

	av = lst_to_matrix(node);
	if (!av)
		return (-1);
	i = 1;
	while (av[i])
	{
		ms_delete_env(env, av[i]);
		i++;
	}
	if (*env != node->head->env_lst)
	{
		aux = node->head;
		while (aux)
		{
			aux->env_lst = *env;
			aux = aux->next;
		}
	}
	free_matrix(av);
	return (0);
}

void	ms_delete_env(t_list **node, char *ref)
{
	char	*env_key;
	t_list	*aux;

	if (!node || !*node)
		return ;
	aux = *node;
	env_key = ft_substr(aux->content, 0,
			ft_strchr(aux->content, '=') - (unsigned long int)aux->content);
	if (!ft_strncmp(ref, env_key, ft_strlen(env_key) + 1))
	{
		*node = aux->next;
		free(aux->content);
		free(aux);
		free(env_key);
		return ;
	}
	else
		ms_delete_env(&aux->next, ref);
	free(env_key);
	return ;
}

int	ms_env(t_word *node)
{
	t_list	*env;

	if (node)
		env = node->env_lst;
	else
		env = NULL;
	while (env)
	{
		ft_putstr_fd(env->content, node->fd_out);
		ft_putchar_fd('\n', node->fd_out);
		env = env->next;
	}
	return (0);
}

int	ms_pwd(t_word *node)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_putstr_fd(cwd, node->fd_out);
	ft_putchar_fd('\n', node->fd_out);
	free(cwd);
	return (0);
}

int	ms_export(t_word *node)
{
	char	**av;
	int		i;

	av = lst_to_matrix(node);
	if (!av)
		return (-1);
	i = 1;
	while (av[i])
	{
		ms_export_util(av[i], node);
		i++;
	}
	free(av[0]);
	free(av);
	return (0);
}

static void	ms_export_util(char *arg, t_word *node)
{
	char	**env_node;
	char	*env_name;

	if (arg[0] != '=' && ft_strchr(arg, '='))
	{
		env_name = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
		env_node = get_environment_node(node->env_lst, env_name);
		if (env_node)
		{
			free(*env_node);
			*env_node = arg;
		}
		else
			ft_lstadd_back(&node->env_lst, ft_lstnew(arg));
		free(env_name);
	}
	else
		free(arg);
}

int	ms_echo(t_word *node)
{
	uint8_t	newline;
	t_word	*head;

	head = node;
	newline = 1;
	if (node->next)
	{
		if (!ft_strncmp(node->next->word, "-n", 2))
		{
			newline = 0;
			node = node->next;
		}
		node = node->next;
		while (node && node->flag != MS_PIPE)
		{
			ft_putstr_fd(node->word, head->fd_out);
			node = node->next;
			if (node && node->flag != MS_PIPE)
				ft_putchar_fd(' ', head->fd_out);
		}
	}
	if (newline)
		ft_putchar_fd('\n', head->fd_out);
	return (0);
}

int	ms_cd(t_word *node)
{
	uint8_t	err;

	if (node->next && node->next->next && node->next->next->flag != MS_WORD)
	{
		ft_putstr_fd("minishell: cd: Too many arguments\n", node->fd_out);
		return (1);
	}
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
	if (chdir(node->next->word) == -1)
	{
		ft_putstr_fd("Ms: cd: no such file or dir\n", STDERR_FILENO);
		free(pwd);
		return (1);
	}
	if (update_environment(node->env_lst, "OLDPWD=", pwd) != 0)
	{
		free(pwd);
		return (1);
	}
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

int	update_environment(t_list *env_lst, char *ref, char *newvalue)
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

static int	str_is_num(char *str)
{
	if (!str || *str == '-')
		return (2);
	if (*str == '+')
		str++;
	if (!*str)
		return (1);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (1);
		str++;
	}
	return (0);
}

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
			ft_free_mat(mat);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (mat);
}

char	**lst_to_matrix(t_word *node)
{
	t_word				*aux;
	char				**mat;
	unsigned int		mat_positions;
	unsigned int		i;

	if (!node)
		return (NULL);
	aux = node;
	i = -1;
	mat_positions = 0;
	while (aux && aux->flag == MS_WORD)
	{
		aux = aux->next;
		mat_positions++;
	}
	mat = ft_calloc(mat_positions + 1, sizeof(char *));
	if (!mat)
		return (NULL);
	while (++i < mat_positions)
	{
		mat[i] = ft_strdup(node->word, 0);
		node = node->next;
	}
	return (mat);
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

char	*check_bin(char *cmd, t_list *env)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd, 0));
		else
		{
			return (ft_putstr_fd("Ms: cmd not found\n", STDERR_FILENO), NULL);
		}
	}
	cmd = ft_strjoin("/", cmd, 0);
	if (check_bin_current_dir(&cmd) == 0)
		return (cmd);
	else if (check_bin_path(&cmd, env) == 0)
		return (cmd);
	free(cmd);
	return (NULL);
}

static int	check_bin_current_dir(char **cmd)
{
	char				*cmd_with_pwd;
	extern unsigned int	g_exit_status;

	cmd_with_pwd = ft_strjoin(getcwd(NULL, 0), *cmd, 1);
	if (access(cmd_with_pwd, F_OK) == 0)
	{
		if (access(cmd_with_pwd, F_OK | X_OK) == 0)
		{
			free(*cmd);
			*cmd = cmd_with_pwd;
			return (0);
		}
		else
		{
			ft_putstr_fd("Ms: cmd access dennied\n", STDERR_FILENO);
			g_exit_status = 126;
		}
	}
	free(cmd_with_pwd);
	return (1);
}

static int	check_bin_path(char **cmd, t_list *env)
{
	int					i;
	char				**path;
	char				*path_cmd;
	extern unsigned int	g_exit_status;

	if (!get_environment_node(env, "PATH"))
	{
		g_exit_status = 127;
		return (ft_putstr_fd("Ms: PATH unseted\n", STDERR_FILENO), 1);
	}
	i = -1;
	path = ft_split(get_environment_node(env, "PATH"), ':');
	while (path[++i])
	{
		path_cmd = ft_strjoin(path[i], *cmd, 0);
		if (access(path_cmd, F_OK | X_OK) == 0)
		{
			free_matrix(path);
			free(*cmd);
			return (*cmd = path_cmd, 0);
		}
		free(path_cmd);
	}
	ft_putstr_fd("Ms: Command not found\n", STDERR_FILENO);
	return (free_matrix(path), g_exit_status = 127, 1);
}

void	free_matrix(char **mat)
{
	int	i;

	i = 0;
	if (!mat)
		return ;
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat);
	return ;
}

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

void	lst_remove_if(t_word **head)
{
	t_word	*node;

	if (!head || !*head || (*head)->flag == MS_PIPE)
		return ;
	node = *head;
	if (node->flag != MS_WORD)
	{
		*head = node->next;
		free(node->word);
		free(node);
		lst_remove_if(head);
	}
	node = *head;
	if (!head || !*head || (*head)->flag == MS_PIPE)
		return ;
	lst_remove_if(&node->next);
	return ;
}

t_word	*clean_sentence_redirections(t_word **lst, int flag)
{
	t_word	*ref;
	t_word	*aux;

	ref = *lst;
	lst_remove_if(lst);
	if (flag && *lst && *lst != ref)
	{
		aux = *lst;
		while (aux)
		{
			aux->head = *lst;
			aux = aux->next;
		}
	}
	return (*lst);
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

int	ms_redirect_in(t_word *node)
{
	t_word	*head;

	head = node;
	while (head && head->flag != MS_WORD)
		head = head->next;
	while (head && node && node->flag != MS_PIPE)
	{
		if (node->flag == MS_REDIRECT_IN)
		{
			if (!access(node->next->word, F_OK | R_OK))
				head->fd_in = open(node->next->word, O_RDWR);
			else
			{
				printf("Error: No such file as '%s'\n", node->next->word);
				return (-1);
			}
		}
		else if (node->flag == MS_HEREDOC)
			head->fd_in = ms_heredoc(node);
		node = node->next;
	}
	return (0);
}

int	ms_do_redirections(t_word *node)
{
	if (!node)
		return (0);
	if (ms_redirect_in(node) || ms_redirect_out(node))
		return (-1);
	return (0);
}

int	has_pipe(t_word *node)
{
	while (node)
	{
		if (node->flag == MS_PIPE)
			return (0);
		node = node->next;
	}
	return (1);
}

int	ms_pipe(t_word *node)
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

void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
	return ;
}

int	get_flag_word(char *word, int last_flag)
{
	if (!word)
		return (-1);
	if (ft_strncmp(word, "|", 2) == 0)
		return (MS_PIPE);
	else if (ft_strncmp(word, "<<", 3) == 0)
		return (MS_HEREDOC);
	else if (ft_strncmp(word, ">>", 3) == 0)
		return (MS_APPEND);
	else if (ft_strncmp(word, "<", 2) == 0)
		return (MS_REDIRECT_IN);
	else if (ft_strncmp(word, ">", 2) == 0)
		return (MS_REDIRECT_OUT);
	else if (last_flag == MS_REDIRECT_OUT || last_flag == MS_REDIRECT_IN
		|| last_flag == MS_HEREDOC || last_flag == MS_APPEND)
		return (MS_REDIRECT_FILE);
	return (MS_WORD);
}

int	valid_environment_name(char *line)
{
	if (line && line[1] == '?')
		return (0);
	if (!line || *line != '$' || !ft_isalnum(line[1]))
		return (1);
	return (0);
}

int	get_len_decimal(int n)
{
	int	len;

	len = 0;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

int	get_environment_name_len(char *line)
{
	int	word_len;

	if (!line)
		return (0);
	word_len = 0;
	line++;
	if (*line == '?')
		return (1);
	while (line && *line && ft_isalpha(*line))
	{
		word_len++;
		line++;
	}
	return (word_len);
}

char	*get_environment_node(t_list *env_node, char *env)
{
	char	*env_key;
	char	*env_line;

	env_line = NULL;
	while (env_node)
	{
		env_line = env_node->content;
		env_key = ft_substr(env_node->content,
				0, ft_strchr(env_line, '=') - env_line);
		if (!ft_strncmp(env_key, env, ft_strlen(env_key) + 1))
		{
			free(env_key);
			break ;
		}
		free(env_key);
		env_node = env_node->next;
	}
	if (!env_node)
		return (NULL);
	if (env_line)
		env_line = ft_strchr(env_line, '=') + 1;
	return (env_line);
}

void	expand_environment_utils_check_line(char *line, char *ret_line, t_pos *aux, t_list *env)
{
	char					*env_value;
	int						env_len;
	extern unsigned int		g_exit_status;

	if (line[aux->i + 1] == '?')
	{
		if (g_exit_status == 0)
			env_len = 1;
		else
			env_len = count_decimal(g_exit_status);
		env_value = ft_itoa(g_exit_status);
		ft_memcpy(&ret_line[aux->j], env_value, env_len);
	}
	else
	{
		env_value = ft_substr(&line[aux->i], 1,
				get_environment_name_len(&line[aux->i]));
		env_len = ft_strlen(get_environment_node(env, env_value));
		ft_memcpy(&ret_line[aux->j], get_environment_node(env, env_value), env_len);
	}
	aux->i += get_environment_name_len(&line[aux->i]) + 1;
	aux->j += env_len;
	free(env_value);
	return ;
}

void	expand_environment_util(char *line, char *ret_line, t_list *env)
{
	t_pos	aux;
	char	must_expand;
	char	in_double_quotes;

	aux = (t_pos){0};
	must_expand = 1;
	in_double_quotes = 1;
	while (line[aux.i])
	{
		if (line[aux.i] == '\"' && must_expand)
			in_double_quotes ^= 1;
		if (line[aux.i] == '\'' && in_double_quotes)
			must_expand ^= 1;
		if (line[aux.i] == '$' && must_expand
			&& !valid_environment_name(&line[aux.i]))
			expand_environment_utils_check_line(line, ret_line, &aux, env);
		else
		{
			ret_line[aux.j] = line[aux.i];
			aux.i++;
			aux.j++;
		}
	}
	return ;
}

int	get_expand_environment_len(char *line, t_list *env)
{
	char				*env_word;
	int					len;
	extern unsigned int	g_exit_status;

	if (line && line[1] == '?')
	{
		if (g_exit_status == 0)
			return (1);
		else
			return (count_decimal(g_exit_status));
	}
	env_word = ft_substr(line, 1, get_environment_name_len(line));
	len = ft_strlen(get_environment_node(env, env_word));
	if (env_word)
		free(env_word);
	return (len);
}

int	get_environment_len_after_expand(char *line, t_list *env)
{
	int		final_len;
	char	must_expand;
	char	in_double_quotes;

	in_double_quotes = 0;
	must_expand = 1;
	final_len = 0;
	while (*line)
	{
		if (*line == '\"' && must_expand)
			in_double_quotes ^= 1;
		if (*line == '\'' && !in_double_quotes)
			must_expand ^= 1;
		if (*line == '$' && must_expand && !valid_environment_name(line))
		{
			final_len += get_expand_environment_len(line, env);
			line += get_environment_name_len(line) + 1;
		}
		else
		{
			final_len++;
			line++;
		}
	}
	return (final_len);
}

char	*expand_environment(char *line, t_list *env)
{
	char	*ret_line;

	ret_line = ft_calloc(get_environment_len_after_expand(line, env) + 1,
			sizeof(char));
	if (ret_line)
		expand_environment_util(line, ret_line, env);
	free(line);
	return (ret_line);
}

static void	set_quote_with_null(char *line)
{
	int	next_quotes;

	if (!line)
		return ;
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			next_quotes = get_next_quotes(line);
			*line = '\0';
			line += next_quotes;
			*line = '\0';
			line++;
		}
		else
			line++;
	}
	return ;
}

char	*remove_quotes(char *line, int flag)
{
	char	*ret_word;
	int		i;
	int		len;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	ret_word = NULL;
	i = 0;
	if (!(ft_strchr(line, '\'') || ft_strchr(line, '\"')))
		return (line);
	set_quote_with_null(line);
	while (!line[i] && i < len)
		i++;
	while (i < len)
	{
		ret_word = ft_strjoin(ret_word, &line[i], 1);
		i += ft_strlen(&line[i]) + 1;
		while (!line[i] && i < len)
			i++;
	}
	if (flag)
		free(line);
	return (ret_word);
}

int	lexx(t_word **lst)
{
	t_word	*node;
	int		last_flag;

	if (!lst || !*lst)
		return (1);
	node = *lst;
	last_flag = 0;
	while (node)
	{
		if (node->word)
		{
			node->flag = get_flag_word(node->word, last_flag);
			node->word = expand_environment(node->word, node->env_lst);
			node->word = remove_quotes(node->word, 1);
			last_flag = node->flag;
		}
		node = node->next;
	}
	ms_lst_remove_empty_word(lst);
	return (0);
}

int	analyze_pipe_syntax(t_word *word_lst)
{
	t_word	*node;
	int		last_flag;

	if (!word_lst)
		return (-1);
	node = word_lst;
	last_flag = MS_PIPE;
	while (node)
	{
		if ((last_flag == MS_PIPE && node->flag == MS_PIPE)
			|| (!node->next && node->flag == MS_PIPE))
		{
			printf("Syntax error: Incorrect use of pipes\n");
			return (-1);
		}
		last_flag = node->flag;
		node = node->next;
	}
	return (0);
}

int	analyze_redirect_syntax(t_word *word_lst)
{
	t_word	*node;

	if (!word_lst)
		return (-1);
	node = word_lst;
	while (node)
	{
		if (node->flag == MS_REDIRECT_IN || node->flag == MS_REDIRECT_OUT
			|| node->flag == MS_HEREDOC || node->flag == MS_APPEND)
		{
			if (!node->next || node->next->flag != MS_REDIRECT_FILE)
			{
				printf("Syntax error: error on '%s'\n", node->word);
				return (-1);
			}
		}
		node = node->next;
	}
	return (0);
}

int	syntax_analyze(t_word *word_lst)
{
	if (!word_lst)
		return (-1);
	else if (analyze_pipe_syntax(word_lst))
		return (-1);
	else if (analyze_redirect_syntax(word_lst))
		return (-1);
	return (0);
}

int	get_command_count(t_word *node)
{
	int	cmd_count;

	cmd_count = 0;
	while (node && node->flag != MS_PIPE)
	{
		if (node->flag == MS_WORD)
			cmd_count++;
		node = node->next;
	}
	return (cmd_count);
}

int	analyze_command_syntax(t_word *word_lst)
{
	while (word_lst)
	{
		if (get_command_count(word_lst) == 0)
		{
			ft_putstr_fd("Syntax error: sentence w/no command\n", STDERR_FILENO);
			return (1);
		}
		while (word_lst && word_lst->flag != MS_PIPE)
			word_lst = word_lst->next;
		if (word_lst)
			word_lst = word_lst->next;
	}
	return (0);
}

int	lexical_analyzer(t_word **word_lst)
{
	if (!word_lst)
		return (1);
	if (lexx(word_lst))
		return (1);
	if (syntax_analyze(*word_lst))
		return (1);
	if (analyze_command_syntax(*word_lst))
		return (1);
	return (0);
}

int	parser(char *line, t_data *data)
{
	int error;

	error = 0;
	if (check_quote(line))
	{
		printf("Syntax error: There is an unclosed quote\n");
		free(line);
		return (-1);
	}
	data->prompt = parse_prompt(line, data->environ);
	error = lexical_analyzer(data->prompt);
	free(line);
	return (error);
}

static void	set_head(t_word *head)
{
	t_word	*aux;

	if (!head)
		return ;
	aux = head;
	while (aux)
	{
		aux->head = head;
		aux = aux->next;
	}
	return ;
}

t_word	*parse_prompt(char *line, t_list *env_lst)
{
	int		word_len;
	t_word	*new_word;
	t_word	*word_lst;

	if (!line)
		return (NULL);
	word_lst = NULL;
	while (ft_isspace(*line))
		line++;
	while (*line)
	{
		word_len = get_word_len(line);
		new_word = ms_lstnew(ft_substr(line, 0, word_len));
		new_word->env_lst = env_lst;
		ms_lstadd_back(&word_lst, new_word);
		line += word_len;
		while (ft_isspace(*line))
			line++;
	}
	set_head(word_lst);
	return (word_lst);
}

int	get_word_len(char *line)
{
	int	word_len;

	if (is_meta_char(line))
		return (is_meta_char(line));
	word_len = 0;
	while (*line && !is_meta_char(line) && !ft_isspace(*line))
	{
		if (*line == '\'' || *line == '\"')
		{
			word_len += get_next_quotes(line) + 1;
			line += get_next_quotes(line) + 1;
		}
		else
		{
			word_len++;
			line++;
		}
	}
	return (word_len);
}

int	is_meta_char(char *c)
{
	if ((c[0] == '<' && c[1] == '<') || (c[0] == '>' && c[1] == '>'))
		return (2);
	else if (*c == '|' || *c == '>' || *c == '<')
		return (1);
	return (0);
}

void	ms_lstclear(t_word **lst, int flag)
{
	t_word	*aux;
	t_word	*node;

	if (!lst || !(*lst))
		return ;
	node = *lst;
	while (node)
	{
		aux = node->next;
		free(node->word);
		free(node);
		node = aux;
	}
	if (flag)
		*lst = NULL;
	return ;
}

t_word	*ms_lstnew(void *word)
{
	t_word	*new;

	new = (t_word *)malloc(sizeof(t_word));
	if (!new)
		return (NULL);
	new->word = word;
	new->flag = 0;
	new->pid = 0;
	new->fd_in = 0;
	new->fd_out = 1;
	new->ret = -1;
	new->next = NULL;
	return (new);
}

void	ms_lstadd_back(t_word **lst, t_word *new)
{
	t_word	*last_node;

	if (!new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		last_node = *lst;
		while (last_node->next)
			last_node = last_node->next;
		last_node->next = new;
	}
	return ;
}

int	check_quote(char *line)
{
	int	quote_distance;

	if (!line)
		return (1);
	quote_distance = 0;
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			quote_distance = get_next_quotes(line);
			if (quote_distance == -1)
				return (1);
			line += quote_distance + 1;
		}
		else
			line++;
	}
	return (0);
}

int	get_next_quotes(char *line)
{
	char	quote;
	int		next_quote_distance;

	if (!line || !*line)
		return (0);
	quote = *line;
	line++;
	next_quote_distance = 1;
	while (*line != quote)
	{
		if (!*line)
			return (-1);
		next_quote_distance++;
		line++;
	}
	return (next_quote_distance);
}