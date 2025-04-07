/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 21:51:52 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 16:25:03 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
		i++;
	dup = (char *)ft_calloc((i + 1), 1);
	if (dup == NULL)
		return (NULL);
	while (i >= 0)
	{
		dup[i] = s[i];
		i--;
	}
	return (dup);
}
