/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_step.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 18:52:11 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 10:54:30 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	to_nb_cmd(t_shell *shell)
{
	close(shell->fd[1]);
	shell->fd_temp = shell->fd[0];
	pipe(shell->fd);
}

void	from_nb_cmd(t_shell *shell)
{
	close(shell->fd_temp);
	close(shell->fd[1]);
	shell->fd_temp = shell->fd[0];
}

void	step_1_next(t_token *token, t_shell *shell, char **cmd_exec)
{
	if (check_cmd_quotes(cmd_exec[0]) == 2)
		echo(token, 0);
	else if (check_cmd_quotes(cmd_exec[0]) == 3)
		cd(shell, token[1].str);
	else if (check_cmd_quotes(cmd_exec[0]) == 4)
		pwd();
	else if (check_cmd_quotes(cmd_exec[0]) == 5)
		export(shell, token, 0);
	else if (check_cmd_quotes(cmd_exec[0]) == 6)
		unset(shell, token, 0);
	else if (check_cmd_quotes(cmd_exec[0]) == 7)
		env(shell);
	else if (check_cmd_quotes(cmd_exec[0]) == 8)
		ft_exit(token, 0);
	exit(0);
}

void	step_1(pid_t pid, t_token *token, t_shell *shell)
{
	int		i;
	char	*path;
	char	**cmd_exec;

	i = 0;
	if (pid == 0)
	{
		dup2(shell->fd[1], STDOUT_FILENO);
		close(shell->fd[0]);
		close(shell->fd[1]);
		shell->index = 0;
		cmd_exec = malloc((token_nb(token, 0) + 1) * sizeof(char *));
		while (token[shell->index].id != 6)
		{
			cmd_exec[i] = ft_strdup(token[shell->index].str);
			i++;
			shell->index++;
		}
		cmd_exec[i] = NULL;
		verif_before_exec(token, shell, cmd_exec);
		path = make_path(cmd_exec[0], shell);
		execve(path, cmd_exec, NULL);
		exit(2);
	}
}

void	last_step(t_shell *shell, t_token *token, pid_t pid)
{
	int		i;

	i = shell->index;
	if (pid == 0)
	{
		dup2(shell->fd[0], STDIN_FILENO);
		close(shell->fd[1]);
		close(shell->fd[0]);
		while (token[i].str)
		{
			if (token[i].id == 4)
				last_step_4(token, i);
			else if (token[i].id == 40)
				last_step_40(token, i);
			i++;
		}
		command_execution(token, shell);
		exit(0);
	}
	close(shell->fd[0]);
}
