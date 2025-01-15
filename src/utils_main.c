/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/15 16:49:20 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int contains_special_char(t_cmd *cmd)
{
	int i;

	i = 0;
    while (cmd->args[i])
	{
        
        if (strchr(cmd->args[i], '|') || strchr(cmd->args[i], '<')
			|| strchr(cmd->args[i], '>'))
		{
            return (1);
        }
		i++;
    }
    return (0);
}
