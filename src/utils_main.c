/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/25 18:51:58 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int contains_special_char(t_cmd *cmd, char delimiter)
{
	int i;

	if (!cmd || !cmd->args)
        return (0);
	i = 0;
    while (cmd->args[i])
	{
        
        if (ft_strchr(cmd->args[i], delimiter)) 
		{
            return (1);
        }
		i++;
    }
    return (0);
}
