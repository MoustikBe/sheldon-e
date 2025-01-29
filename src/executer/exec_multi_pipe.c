/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:52:11 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/29 12:32:46 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**joining_command(t_token *token, t_shell *shell, t_utils *u)
{
	char	**cmd_exec;
	int		i;

	i = 0;
	cmd_exec = malloc((token_nb(token, shell->index) + 1) * sizeof(char *));
	while (token[shell->index].id != 6 && token[shell->index].str)
	{
		if (token[shell->index].id == 4)
			command_exec_4(shell, token);
		else if (token[shell->index].id == 5 && str_cmp(token[u->i_copy].str,
				"cat") == 0 && str_cmp(token[u->i_copy].str, "/bin/cat") == 0)
			command_exec_5(shell, token);
		else if (token[shell->index].id == 40)
			command_exec_40(shell, token);
		else
		{
			cmd_exec[i] = ft_strdup(token[shell->index].str);
			shell->index++;
			i++;
		}
	}
	cmd_exec[i] = NULL;
	return (cmd_exec);
}

static int	len_array(t_shell *shell)
{
	int		i;
	char	**split_cmd;

	i = 0;
	split_cmd = ft_split(shell->cmd, '|');
	while (split_cmd[i])
	{
		free(split_cmd[i]);
		i++;
	}
	free(split_cmd);
	return (i);
}

void	pipex_multi(t_token *token, t_shell *shell)
{
	int		i;
	pid_t	pid;

	shell->index = 0;
	i = len_array(shell);
	shell->nb_cmd = i - 2;
	pipe(shell->fd);
	pid = fork();
	if (pid == -1)
		printf("ERROR\n");
	step_1(pid, token, shell);
	close(shell->fd[1]);
	to_nb_cmd(shell);
	sub_process(shell, token, pid);
	from_nb_cmd(shell);
	pid = fork();
	if (pid == -1)
		exit(2);
	last_step(shell, token, pid);
	wait_execution(shell->nb_cmd + 3);
	close(shell->fd_temp);
	return ;
}
