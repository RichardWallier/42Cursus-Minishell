/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/05 12:57:05 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote(char *line);
int	next_quote(char *line);
t_word	*ms_lstnew(void *word);
void	ms_lstadd_back(t_word **lst, t_word *new);
t_word	*parse_prompt(char *line, t_list *env_lst);
int	parser(char *line, t_data *data);
int	is_meta_char(char *c);

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
		data.bash = ft_strjoin(data.pwd, "$ ");
		line = readline(data.bash);
		free(data.bash);
		if (!line)
			exit_builtin(&data);
		if (!is_only_whitespaces(line))
			add_history(line);
		parser(&line, &data);
		// if (ft_strncmp(*data.prompt, "cd", ft_strlen(*data.prompt)) == 0)
		// 	cd_builtin(data.prompt, &data);
		// else if (ft_strncmp(*data.prompt, "env", ft_strlen(*data.prompt)) == 0)
		// 	env_builtin(&data);
		// else if (ft_strncmp(*data.prompt, "export", ft_strlen(*data.prompt)) == 0)
		// 	export_builtin(data.prompt, &data);
		// else if (ft_strncmp(*data.prompt, "unset", ft_strlen(*data.prompt)) == 0)
		// 	unset_builtin(data.prompt, &data);
		// else if (ft_strncmp(*data.prompt, "exit", ft_strlen(*data.prompt)) == 0)
			// exit_builtin(&data);
		// else
		// {
			pid = fork();
			if (pid == 0)
				run_commands(data.prompt, environ);
		// }
		waitpid(pid, NULL, 0);
		free_split(data.prompt);
	}
	return (0);
}

int	ms_lexxer(t_word **word_lst)
{
	if (!word_lst)
		return (1);
	if (ms_analyze_lexx(word_lst))
		return (1);
	if (ms_analyze_syntax(*word_lst))
		return (1);
	if (ms_analyze_cmd_syntax(*word_lst))
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
	error = ms_lexxer(data->prompt); // TODO
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
			word_len += next_quote(line) + 1;
			line += next_quote(line) + 1;
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
			quote_distance = next_quote(line);
			if (quote_distance == -1)
				return (1);
			line += quote_distance + 1;
		}
		else
			line++;
	}
	return (0);
}

int	next_quote(char *line)
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