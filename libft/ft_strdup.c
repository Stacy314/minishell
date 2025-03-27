/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 21:51:52 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/25 16:28:00 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	if(!s)
		return (NULL);
	i = 0;
	while (s[i])
		i++;
	dup = (char *)malloc(i + 1);
	if (dup == NULL)
		return (NULL);
	while (i >= 0)
	{
		dup[i] = s[i];
		i--;
	}
	return (dup);
}
