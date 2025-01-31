/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multi_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:52:33 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 18:43:42 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	sub_process(t_shell *shell, t_token *token, pid_t pid)
{
	if (shell->index == 0)
	{
		while (token[shell->index].id != 6)
			shell->index++;
	}
	shell->index++;
	while (shell->nb_cmd)
	{
		pid = fork();
		if (pid == -1)
			exit(2);
		if (token[shell->index].id == 6)
			shell->index++;
		createprocessus(pid, shell, token);
	}
}

void	createprocessus(pid_t pid, t_shell *shell, t_token *token)
{
	if (pid == 0)
	{
		close(shell->fd[0]);
		dup2(shell->fd_temp, STDIN_FILENO);
		dup2(shell->fd[1], STDOUT_FILENO);
		close(shell->fd_temp);
		close(shell->fd[1]);
		command_execution(token, shell);
	}
	if (shell->nb_cmd > 1)
	{
		close(shell->fd_temp);
		close(shell->fd[1]);
		shell->fd_temp = shell->fd[0];
		pipe(shell->fd);
	}
	shell->nb_cmd--;
	while (token[shell->index].id != 6)
		shell->index++;
	shell->index++;
}

void	wait_execution(int nb_cmd)
{
	int	status;

	while (nb_cmd--)
	{
		waitpid(-1, &status, 0);
	}
}

void	command_execution_next(t_shell *shell, t_token *token,
char **cmd_exec, int i_copy)
{
	if (check_cmd_quotes(cmd_exec[0]) == 2)
		echo(token, i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 3)
		cd(shell, token[1].str);
	else if (check_cmd_quotes(cmd_exec[0]) == 4)
		pwd();
	else if (check_cmd_quotes(cmd_exec[0]) == 5)
		export(shell, token, i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 6)
		unset(shell, token, i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 7)
		env(shell);
	else if (check_cmd_quotes(cmd_exec[0]) == 8)
		ft_exit(token, i_copy);
	exit(0);
}

void	command_execution(t_token *token, t_shell *shell)
{
	char	*path;
	char	**cmd_exec;
	t_utils	*u;

	u = malloc(sizeof(t_utils));
	init_var_utils(u);
	u->i_copy = shell->index;
	cmd_exec = joining_command(token, shell, u);
	if (check_cmd_quotes(cmd_exec[0]) > 1)
		command_execution_next(shell, token, cmd_exec, u->i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 0)
	{
		write(2, cmd_exec[0], strlen(cmd_exec[0]));
		write(2, ": command not found\n", 20);
		shell->last_exit_status = 127;
		exit(2);
	}
	path = make_path(cmd_exec[0], shell);
	execve(path, cmd_exec, NULL);
}
