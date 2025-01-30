/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 20:26:59 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 13:16:00 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_pipe(t_token *token)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (token[i].str)
	{
		if (token[i].id == 6)
			count++;
		i++;
	}
	if (count == 1)
		return (1);
	else if (count > 1)
		return (2);
	return (0);
}

void	verif_in(t_token *token, int i)
{
	char	*try_in;

	try_in = clean_name(token[i].str);
	if (access(try_in, O_RDONLY) != 0)
	{
		printf("bash: %s: No such file or directory\n", try_in);
		free(try_in);
		exit(1);
	}
	free(try_in);
}

int	token_nb(t_token *token, int i)
{
	int	len;

	len = 0;
	while (token[i].str && token[i].id != 6)
	{
		len++;
		i++;
	}
	return (i);
}

void	verif_before_exec(t_token *token, t_shell *shell, char	**cmd_exec)
{
	if (check_cmd_quotes(cmd_exec[0]) > 1)
		step_1_next(token, shell, cmd_exec);
	else if (check_cmd_quotes(cmd_exec[0]) == 0)
	{
		write(2, cmd_exec[0], strlen(cmd_exec[0]));
		write(2, ": command not found\n", 20);
		exit (0);
	}
}
