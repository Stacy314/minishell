/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 15:39:14 by apechkov          #+#    #+#             */
/*   Updated: 2024/04/30 15:39:16 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new1)
{
	if (new1 == 0)
		return ;
	new1->next = *lst;
	*lst = new1;
}

/*int main()
{
    // Creating a linked list manually
    t_list *head = ft_lstnew("second");
    ft_lstadd_front(&head, ft_lstnew("first"));

    // Printing the content of the linked list
    printf("Content of the linked list:\n");
    t_list *current = head;
    while (current)
    {
        printf("%s\n", (char *)current->content);
        current = current->next;
    }

    // Freeing the memory allocated for the linked list
    current = head;
    while (current)
    {
        t_list *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}*/
