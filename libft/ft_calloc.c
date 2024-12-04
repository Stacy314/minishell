/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:50:16 by apechkov          #+#    #+#             */
/*   Updated: 2024/04/24 16:57:34 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*ptr;
	size_t	length;

	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	length = size * nmemb;
	ptr = malloc(length);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, length);
	return (ptr);
}
/*int main(void)
{
    size_t nmemb = 5;
    size_t size = sizeof(int);
    int *arr = (int *)calloc(nmemb, size);

    if (arr != NULL)
    {
        printf("Memory allocated successfully:\n");
        for (size_t i = 0; i < nmemb; i++)
        {
            printf("arr[%zu] = %d\n", i, arr[i]);
        }
        free(arr);
    }
    else
    {
        printf("Memory allocation failed\n");
    }

    return 0;
}*/
