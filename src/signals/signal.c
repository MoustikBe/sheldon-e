/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:02:20 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/27 16:56:47 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
int	g_signal = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	manage_signals(t_shell *shell)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_handler);
	if(g_signal == 1)
	{
		shell->last_exit_status = 130;
		g_signal = 0;
	}
}
