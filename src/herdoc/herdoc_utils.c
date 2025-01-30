/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:20:40 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 19:05:23 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	heredoc_sig(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	child_gnl_exec(t_shell *shell, t_utils *utils,
char *gnl_val, char *delemiter)
{
	while (1)
	{
		signal(SIGINT, &heredoc_sig);
		gnl_val = get_next_line(0);
		if (gnl_val == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (str_cmp(gnl_val, delemiter) == 1)
			break ;
		if (check_env_var(gnl_val) == 1)
		{
			heredoc_expansion(shell, gnl_val, utils->temp_fd);
			write(0, "> ", 2);
		}
		else
		{
			write(utils->temp_fd, gnl_val, ft_strlen(gnl_val));
			write(0, "> ", 2);
		}
		free(gnl_val);
	}
	free(gnl_val);
}

void	cal_len_heredoc(t_shell *shell, t_utils *utils)
{
	while (shell->cmd[utils->i])
	{
		if (shell->cmd[utils->i] == '<')
		{
			utils->i = utils->i + 2;
			if (shell->cmd[utils->i] == ' ')
				utils->i++;
			while (shell->cmd[utils->i] != ' ')
				utils->i++;
		}
		else
		{
			utils->len++;
			utils->i++;
		}
	}
}

void	copy_heredoc(t_shell *shell, t_utils *utils,
char *new_cmd, char *tmp_file)
{
	while (shell->cmd[utils->i])
	{
		if (shell->cmd[utils->i] == '<')
		{
			new_cmd[utils->len] = shell->cmd[utils->i];
			utils->len++;
			utils->i = utils->i + 2;
			if (shell->cmd[utils->i] == ' ')
				utils->i++;
			while (shell->cmd[utils->i] != ' ')
				utils->i++;
			while (tmp_file[utils->j])
			{
				new_cmd[utils->len] = tmp_file[utils->j];
				utils->len++;
				utils->j++;
			}
		}
		else
		{
			new_cmd[utils->len] = shell->cmd[utils->i];
			utils->len++;
			utils->i++;
		}
	}
}

int	check_env_var(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}
