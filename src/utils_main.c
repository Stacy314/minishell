/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/20 17:09:48 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int contains_special_char(t_cmd *cmd, char delimiter) //change name
{
	int i;

	if (!cmd || !cmd->args) // **Перевіряємо, чи `cmd` не `NULL`**
        return 0;
	i = 0;
    while (cmd->args[i])
	{
        
        if (strchr(cmd->args[i], delimiter)) //|| strchr(cmd->args[i], '<')
			//|| strchr(cmd->args[i], '>'))
		{
            return (1);
        }
		i++;
    }
    return (0);
}


