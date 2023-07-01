/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 21:26:13 by rwallier          #+#    #+#             */
/*   Updated: 2023/06/29 10:23:58 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_environment_utils_check_line(char *line,
		char *ret_line, t_pos *aux, t_list *env)
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
		ft_memcpy(&ret_line[aux->j],
			get_environment_node(env, env_value), env_len);
	}
	aux->i += get_environment_name_len(&line[aux->i]) + 1;
	aux->j += env_len;
	free(env_value);
	return ;
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

int	valid_environment_name(char *line)
{
	if (line && line[1] == '?')
		return (0);
	if (!line || *line != '$' || !ft_isalnum(line[1]))
		return (1);
	return (0);
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
			return (ft_putstr_fd("Ms: cmd not found\n", STDERR_FILENO), NULL);
	}
	cmd = ft_strjoin("/", cmd, 0);
	if (check_bin_current_dir(&cmd) == 0)
		return (cmd);
	else if (check_bin_path(&cmd, env) == 0)
		return (cmd);
	free(cmd);
	return (NULL);
}

int	check_bin_current_dir(char **cmd)
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
			ft_putstr_fd("Ms: cmd access denied\n", STDERR_FILENO);
			g_exit_status = 126;
			return (0);
		}
	}
	free(cmd_with_pwd);
	return (1);
}
