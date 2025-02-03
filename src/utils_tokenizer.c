/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/27 14:28:40 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
	
void	free_tokens(t_token **tokens)
{
	int i;

	i = 0;
	if (!tokens)
        return ;
   	while (tokens[i])
	{
        free(tokens[i]->value);
		free(tokens[i]);
		i++;
    }
    free(tokens);
}
