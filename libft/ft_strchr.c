/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 19:13:24 by apechkov          #+#    #+#             */
/*   Updated: 2024/04/05 19:13:26 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c)
	{
		if (!s[i])
			return (0);
		i++;
	}
	return ((char *)s + i);
}

/*int main(void)
{
    const char *str = "teste";
    int char_find = 'e'; 

    char *result = strchr(str, char_find);

    if (result != NULL)
        printf("Character '%c' found: %ld\n", char_find, result - str);
    else
        printf("Character '%c' not found.\n", char_find);
   return 0;
}*/
