/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_next.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 19:33:52 by misaac-c          #+#    #+#             */
/*   Updated: 2025/01/28 11:22:16 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	detect_synt(char *src)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (src[i])
	{
		if (src[i] == '>' || src[i] == '<')
			count++;
		i++;
	}
	return (count);
}

char	*clean_name(char *src)
{
	int		length;
	int		count;
	int		i;
	char	*dest;

	length = ft_strlen(src);
	count = 0;
	i = 0;
	if (detect_synt(src))
		dest = malloc((length) + 1 - detect_synt(src) * sizeof(char));
	else
		dest = malloc((length) + 1 * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (src[count] != '\0')
	{
		if (src[count] == '>' || src[count] == '<')
			count++;
		else
			dest[i++] = src[count++];
	}
	dest[i] = '\0';
	free(src);
	return (dest);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	int		length;
	int		count;

	length = ft_strlen(src);
	count = 0;
	dest = malloc((length + 1) * sizeof(char));
	if (dest == NULL)
	{
		return (NULL);
	}
	while (src[count] != '\0')
	{
		dest[count] = src[count];
		count++;
	}
	dest[count] = '\0';
	return (dest);
}

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	count;

	count = 0;
	if (!n)
		return (0);
	while (count < n && s1[count] && s2[count]
		&& (unsigned char)s1[count] == (unsigned char)s2[count])
		count++;
	if (count < n)
		return ((unsigned char)s1[count] - (unsigned char)s2[count]);
	else
		return (0);
}
