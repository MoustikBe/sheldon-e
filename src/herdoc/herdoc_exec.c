/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:20:40 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/27 16:19:28 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	len_delimiter(t_shell *shell, t_utils *utils)
{
	while (shell->cmd[utils->i])
	{
		if (shell->cmd[utils->i] == '<' && shell->cmd[utils->i + 1] == '<')
		{
			utils->i = utils->i + 2;
			while (shell->cmd[utils->i] == ' ')
				utils->i++;
			while (shell->cmd[utils->i] && shell->cmd[utils->i] != ' ')
			{
				utils->len++;
				utils->i++;
			}
			break ;
		}
		utils->i++;
	}
}

static void	copy_delimiter(t_shell *shell, t_utils *utils, char *delemiter)
{
	while (shell->cmd[utils->i])
	{
		if (shell->cmd[utils->i] == '<' && shell->cmd[utils->i + 1] == '<')
		{
			utils->i = utils->i + 2;
			while (shell->cmd[utils->i] == ' ')
				utils->i++;
			while (shell->cmd[utils->i] && shell->cmd[utils->i] != ' ')
			{
				delemiter[utils->len] = shell->cmd[utils->i];
				utils->len++;
				utils->i++;
			}
			break ;
		}
		utils->i++;
	}
}

static void	gnl_exec_herdoc(t_shell *shell, t_utils *utils,
char *gnl_val, char *delemiter)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		child_gnl_exec(shell, utils, gnl_val, delemiter);
		free(delemiter);
		free(gnl_val);
		exit(0);
	}
	else
		wait(0);
}

void	exec_herdoc(t_shell *shell)
{
	char	*gnl_val;
	char	*delemiter;
	t_utils	*utils;

	utils = malloc(sizeof(t_utils));
	init_var_utils(utils);
	len_delimiter(shell, utils);
	delemiter = malloc(sizeof(char) * utils->len + 2);
	utils->i = 0;
	utils->len = 0;
	copy_delimiter(shell, utils, delemiter);
	delemiter[utils->len] = '\n';
	delemiter[utils->len + 1] = '\0';
	utils->temp_fd = open("/tmp/.heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (utils->temp_fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	write(0, "> ", 2);
	gnl_val = NULL;
	gnl_exec_herdoc(shell, utils, gnl_val, delemiter);
	free(utils);
}

void	replace_heredoc(t_shell *shell)
{
	char	*new_cmd;
	char	*tmp_file;
	t_utils	*utils;

	utils = malloc(sizeof(t_utils));
	init_var_utils(utils);
	tmp_file = ft_strdup("/tmp/.heredoc");
	cal_len_heredoc(shell, utils);
	new_cmd = malloc(sizeof(char) * utils->len
			+ ft_strlen("/tmp/.heredoc") + 2);
	utils->len = 0;
	utils->i = 0;
	copy_heredoc(shell, utils, new_cmd, tmp_file);
	free(tmp_file);
	new_cmd[utils->len] = '\0';
	free(shell->cmd);
	shell->cmd = ft_strdup(new_cmd);
	free(new_cmd);
	free(utils);
}
