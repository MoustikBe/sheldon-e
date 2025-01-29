/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:06:06 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/29 12:37:19 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**child_join_char(t_utils *u, t_token *tok)
{
	char	**cmd_exec;
	int		i;

	i = 0;
	cmd_exec = malloc((token_nb(tok, u->i) + 1) * sizeof(char *));
	while (tok[u->i].id != 6)
	{
		if (tok[u->i].id == 4 || (tok[u->i].id == 5
				&& str_cmp(tok[0].str, "cat") == 0
				&& str_cmp(tok[0].str, "/bin/cat") == 0) || tok[u->i].id == 40)
			u->i++;
		else
		{
			cmd_exec[i] = ft_strdup(tok[u->i].str);
			u->i++;
			i++;
		}
	}
	cmd_exec[i] = NULL;
	return (cmd_exec);
}

void	inter_step_pipe(int fd[2])
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
}
/*
static void	parent_join_char(t_utils *utils, t_token *token)
{
	
}
*/

char	**parent_buildcharloop(t_utils *u, t_token *token)
{
	char	**cmd_exec;
	int		i;

	i = 0;
	cmd_exec = malloc(token_nb(token, u->i) * sizeof(char *));
	while (token[u->i].str)
	{
		if (token[u->i].id == 4 || (token[u->i].id == 5
				&& str_cmp(token[u->i_copy].str, "cat") == 0
				&& str_cmp(token[u->i_copy].str, "/bin/cat") == 0)
			|| token[u->i].id == 40)
			u->i++;
		else
		{
			cmd_exec[i] = ft_strdup(token[u->i].str);
			i++;
			u->i++;
		}
	}
	cmd_exec[i] = NULL;
	return (cmd_exec);
}

void	pipex_simple(t_token *token, t_shell *shell)
{
	int		i;
	int		fd[2];
	char	*fd_in;
	pid_t	pid;

	i = 0;
	fd_in = NULL;
	while (token[i].str)
	{
		if (token[i].id == 5)
			fd_in = ft_strdup(token[i].str);
		i++;
	}
	pipe(fd);
	pid = fork();
	if (pid == -1)
		return ;
	else if (pid == 0)
		child_process(fd, token, fd_in, shell);
	else
		parent_process(fd, token, shell);
}
