/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 17:54:47 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/28 12:09:14 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	cd(t_shell *shell, char *path)
{
	int	return_val;

	if (path == NULL)
	{
		path = ft_strdup(shell->home);
		if (path == NULL)
			printf("cd: HOME not set\n");
		else
		{
			return_val = chdir(path);
			if (return_val < 0)
				perror("open");
			free(path);
		}
		return ;
	}
	return_val = chdir(path);
	if (return_val < 0)
		perror("open");
	return ;
}
