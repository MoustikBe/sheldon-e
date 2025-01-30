/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_id_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:02:23 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 16:04:49 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_var(t_token *token, int i)
{
	if (str_cmp("$?", token[i].str) == 1)
	{
		token[i].id = 9;
		return (1);
	}
	if (token[i].str[0] == '$')
	{
		token[i].id = 8;
		return (1);
	}
	return (0);
}

int	is_char(t_token *token, int i)
{
	int	len;

	len = ft_strlen(token[i].str);
	if ((token[i].str[0] == '"' && token[i].str[len - 1] == '"')
		|| (token[i].str[0] == '\'' && token[i].str[len - 1] == '\''))
	{
		token[i].id = 7;
		return (1);
	}
	return (0);
}

int	is_pipe(t_token *token, int i)
{
	if (str_cmp_quotes(token[i].str, "|") == 1)
	{
		token[i].id = 6;
		return (1);
	}
	return (0);
}

int	is_infile(t_token *token, int i, t_shell *shell)
{
	if (shell->flag == 1)
		return (0);
	if (i > 0 && token[i - 1].str[0] == '<' && token[i - 1].str[1] != '<')
		token[i].id = 5;
	else if (token[i].str[0] == '<' && token[i].str[1] != '<')
		token[i].id = 5;
	if (token[i].id == 5)
	{
		token[i].str = clean_name(token[i].str);
		return (1);
	}
	return (0);
}

int	token_copy_loop(t_utils *u, t_token *token, char *cmd, int i)
{
	while (cmd[u->j])
	{
		if (cmd[u->j] == ' ' && u->in_quotes == 0 && u->in_quote == 0)
			break ;
		else if (cmd[u->j] == '"' && u->in_quotes == 0)
			token_copy_flag(u, &u->in_quotes);
		else if (cmd[u->j] == '"' && u->in_quotes == 1)
			toke_reset_quot(u, &u->in_quotes);
		else if (cmd[u->j] == '\'' && u->in_quote == 0)
			token_copy_flag(u, &u->in_quote);
		else if (cmd[u->j] == '\'' && u->in_quote == 1)
			toke_reset_quot(u, &u->in_quote);
		else if (u->j > 1 && cmd[u->j] == '|' && cmd[u->j - 1] == '\''
			|| u->j > 1 && cmd[u->j] == '|' && cmd[u->j - 1] == '"' )
		{
			token[i].str[u->len++] = cmd[u->j++];
			token[i].id = 60;
		}
		else
			token[i].str[u->len++] = cmd[u->j++];
	}
	return (u->flag);
}
