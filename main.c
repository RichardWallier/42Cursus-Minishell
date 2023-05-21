/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 18:52:01 by rwallier          #+#    #+#             */
/*   Updated: 2023/05/20 23:34:01 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void) {
  t_data data;
  int pid;
  char *temp;
  extern char **environ;

  data.pwd = getcwd(NULL, 0);
  parse_env(&data, environ);

  while (42) {
    data.bash = ft_strjoin(data.pwd, "$ ");
    temp = readline(data.bash);
    free(data.bash);
    data.prompt = ft_split2(temp, ' ');
    if (!is_only_whitespaces(temp))
      add_history(temp);
    free(temp);
    if (ft_strncmp(*data.prompt, "cd", ft_strlen(*data.prompt)) == 0)
      cd_builtin(data.prompt, &data);
    else if (ft_strncmp(*data.prompt, "env", ft_strlen(*data.prompt)) == 0)
      env_builtin(&data);
    else if (ft_strncmp(*data.prompt, "export", ft_strlen(*data.prompt)) == 0)
      export_builtin(data.prompt, &data);
    else if (ft_strncmp(*data.prompt, "unset", ft_strlen(*data.prompt)) == 0)
      unset_builtin(data.prompt, &data);
    else if (ft_strncmp(*data.prompt, "echo", ft_strlen(*data.prompt)) == 0)
      echo_builtin(data.prompt, &data);
    else if (ft_strncmp(*data.prompt, "exit", ft_strlen(*data.prompt)) == 0)
      exit_builtin(&data);
    else {
      pid = fork();
      if (pid == 0)
        run_commands(data.prompt, environ);
    }
    waitpid(pid, NULL, 0);
    free_split(data.prompt);
  }
  return (0);
}
