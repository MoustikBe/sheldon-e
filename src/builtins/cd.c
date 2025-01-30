/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:54:47 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/30 16:47:29 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cd(t_shell *shell, char *path)
{
	int	return_val;

	if (path == NULL)
	{
		if (shell->home == NULL)
		{
			printf("cd: HOME not set\n");
			return ;
		}
		else
		{
			path = ft_strdup(shell->home);
			return_val = chdir(path);
			if (return_val < 0)
				perror("open");
			free(path);
			return ;
		}
	}
	return_val = chdir(path);
	if (return_val < 0)
		perror("open");
	return ;
}
