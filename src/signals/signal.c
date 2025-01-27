/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 20:02:20 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/27 16:14:10 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal = 1;
}

void	manage_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sigint_handler);
	if (g_signal)
	{
		//exitcode = 130;
		g_signal = 0;
	}
}
