/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/20 17:31:55 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	int i;
	
	i = 0;
	{
		if (tokens[i]->type == WORD)
		{
			if (ft_strncmp(tokens[i]->value, "echo", 4) == 0)
				return (builtin_echo(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "cd", 2) == 0)
				return (builtin_cd(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "pwd", 3) == 0)
				return (builtin_pwd(cmd));
			else if (ft_strncmp(tokens[i]->value, "export", 6) == 0)
				return (builtin_export(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "unset", 5) == 0)
				return (builtin_unset(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "env", 3) == 0)
				return (builtin_env(data));
			else if (ft_strncmp(tokens[i]->value, "exit", 4) == 0)
				return (builtin_exit(cmd, data));
			else
			{
				data->exit_status = execute_command(cmd->args[0], data, cmd->args, env);
				return ;
			}
		}
		i++;
	}
}
