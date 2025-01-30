/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_submain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:38:12 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 12:52:09 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	child_process(int fd[2], t_token *tok, char *file_in, t_shell *shell)
{
	int		i;
	char	*path;
	char	**cmd_exec;
	t_utils	*u;

	i = 0;
	u = malloc(sizeof(t_utils));
	init_var_utils(u);
	cmd_exec = child_join_char(u, tok);
	if (check_cmd_quotes(cmd_exec[0]) == 0)
	{
		printf("%s: command not found\n", cmd_exec[0]);
		exit(0);
	}
	if (file_in)
		child_process_fd(file_in);
	inter_step_pipe(fd);
	if (check_cmd_quotes(cmd_exec[0]) > 1)
		child_process_menu(shell, tok, cmd_exec);
	path = make_path(cmd_exec[0], shell);
	execve(path, cmd_exec, NULL);
}

void	child_process_menu(t_shell *shell, t_token *token, char **cmd_exec)
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

void	parent_process(int fd[2], t_token *token, t_shell *shell)
{
	t_utils	*u;
	char	**cmd_exec;
	char	*path;

	u = malloc(sizeof(t_utils));
	init_var_utils(u);
	while (token[u->i].id != 6)
		u->i++;
	u->i++;
	u->i_copy = u->i;
	cmd_exec = parent_buildcharloop(u, token);
	parent_inter_step(fd);
	u->i = u->i_copy;
	parent_file_mngt(u, token);
	if (!cmd_exec[0])
		exit(0);
	if (check_cmd_quotes(cmd_exec[0]) == 0)
	{
		printf("%s: command not found\n", cmd_exec[0]);
		exit(0);
	}
	if (check_cmd_quotes(cmd_exec[0]) > 1)
		parent_menu(u, token, shell, cmd_exec);
	path = make_path(cmd_exec[0], shell);
	execve(path, cmd_exec, NULL);
}

void	parent_menu(t_utils *u, t_token *token,
t_shell *shell, char **cmd_exec)
{
	if (check_cmd_quotes(cmd_exec[0]) == 2)
		echo(token, u->i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 3)
		cd(shell, token[1].str);
	else if (check_cmd_quotes(cmd_exec[0]) == 4)
		pwd();
	else if (check_cmd_quotes(cmd_exec[0]) == 5)
		export(shell, token, u->i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 6)
		unset(shell, token, u->i_copy);
	else if (check_cmd_quotes(cmd_exec[0]) == 7)
		env(shell);
	else if (check_cmd_quotes(cmd_exec[0]) == 8)
		ft_exit(token, u->i_copy);
	exit(0);
}

void	parent_inter_step(int fd[2])
{
	wait(0);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
}
