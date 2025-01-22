/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cleaner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:48:08 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/20 13:30:58 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	quote_management(t_shell *shell, t_utils *u)
{
	if (shell->cmd[u->i] == '"' && u->in_quotes == 0)
		u->in_quotes = 1;
	else if (shell->cmd[u->i] == '\'' && u->in_quote == 0)
		u->in_quote = 1;
	else if (shell->cmd[u->i] == '"' && u->in_quotes == 1)
		u->in_quotes = 0;
	else if (shell->cmd[u->i] == '\'' && u->in_quote == 1)
		u->in_quote = 0;
}

static void	cmd_len_else(t_utils *u)
{
	u->i++;
	u->len++;
}

static void	cmd_copy_else(t_utils *u, char *pipe_cmd)
{
	pipe_cmd[u->j] = ' ';
	pipe_cmd[u->j + 1] = '|';
	u->j = u->j + 2;
}

static void	cmd_cleaner_len(t_utils *u, t_shell *shell)
{
	while (shell->cmd[u->i])
	{
		quote_management(shell, u);
		if ((shell->cmd[u->i] == '<' && shell->cmd[u->i + 1] == ' '
				&& u->in_quotes == 0 && u->in_quote == 0)
			|| (shell->cmd[u->i] == '>' && shell->cmd[u->i + 1] == ' '
				&& u->in_quotes == 0 && u->in_quote == 0))
		{
			u->i++;
			while (shell->cmd[u->i] == ' ')
				u->i++;
		}
		else if (shell->cmd[u->i] == '|' && u->in_quotes == 0
			&& u->in_quote == 0)
		{
			if (shell->cmd[u->i - 1] != ' ')
				u->len++;
			if (shell->cmd[u->i + 1] != ' ')
				u->len++;
			u->len++;
			u->i++;
		}
		else
			cmd_len_else(u);
	}
}

static void	cmd_cleaner_cutils(t_utils *u, t_shell *shell, char *pipe_cmd)
{
	pipe_cmd[u->j] = shell->cmd[u->i];
	u->j++;
	u->i++;
	while (shell->cmd[u->i] == ' ')
		u->i++;
}

static void	cmd_cleaner_copy(t_utils *u, t_shell *shell, char *pipe_cmd)
{
	while (shell->cmd[u->i])
	{
		quote_management(shell, u);
		if ((shell->cmd[u->i] == '<' && shell->cmd[u->i + 1] == ' '
				&& u->in_quotes == 0 && u->in_quote == 0)
			|| (shell->cmd[u->i] == '>' && shell->cmd[u->i + 1] == ' '
				&& u->in_quotes == 0 && u->in_quote == 0))
			cmd_cleaner_cutils(u, shell, pipe_cmd);
		else if (shell->cmd[u->i] == '|' && u->in_quote == 0
			&& u->in_quotes == 0)
		{
			if (shell->cmd[u->i - 1] == ' ')
				pipe_cmd[u->j++] = '|';
			else
				cmd_copy_else(u, pipe_cmd);
			if (shell->cmd[u->i + 1] != ' ')
				pipe_cmd[u->j++] = ' ';
			u->i++;
		}
		else
			pipe_cmd[u->j++] = shell->cmd[u->i++];
	}
}

static void	quot_file_checker(t_shell *shell)
{
	int	i;

	i = 0;
	shell->flag = 0;
	while (shell->cmd[i])
	{
		if (i > 0)
			if ((shell->cmd[i] == '<' && shell->cmd[i - 1] == '"')
				|| (shell->cmd[i] == '<' && shell->cmd[i - 1] == '\'')
				|| (shell->cmd[i] == '>' && shell->cmd[i - 1] == '"')
				|| (shell->cmd[i] == '>' && shell->cmd[i - 1] == '\''))
				shell->flag = 1;
		i++;
	}
}

void	cmd_cleaner(t_shell *shell)
{
	char	*pipe_cmd;
	t_utils	*u;

	u = malloc(sizeof(t_utils));
	init_var_utils(u);
	cmd_cleaner_len(u, shell);
	if (shell->cmd[u->i - 1] != ' ')
		u->len++;
	pipe_cmd = malloc(sizeof(char) * u->len + 3);
	u->i = 0;
	u->in_quote = 0;
	u->in_quotes = 0;
	cmd_cleaner_copy(u, shell, pipe_cmd);
	if (shell->cmd[u->i - 1] != ' ')
	{
		pipe_cmd[u->j] = ' ';
		u->j++;
	}
	pipe_cmd[u->j] = '\0';
	free(u);
	free(shell->cmd);
	shell->cmd = ft_strdup(pipe_cmd);
	free(pipe_cmd);
	quot_file_checker(shell);
}

int	verif_quotes(char *cmd)
{
	int	i;
	int	quote_s;
	int	quote_d;

	i = 0;
	quote_s = 0;
	quote_d = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"')
			quote_d++;
		else if (cmd[i] == '\'')
			quote_s++;
		i++;
	}
	if (quote_d % 2 == 0 && quote_s % 2 == 0)
		return (0);
	return (1);
}
