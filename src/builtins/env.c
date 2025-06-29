/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 19:04:43 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_env(t_data *data, t_cmd *cmd, int token_index)
{
	if (!data->env)
		return (data->exit_status = 1);
	if (cmd->args[token_index + 1])
	{
		write_error("env: too many arguments\n");
		return (data->exit_status = 1);
	}
	while (data->env[token_index])
	{
		if (ft_strchr(data->env[token_index], '='))
		{
			ft_putstr_fd(data->env[token_index], 1);
			ft_putstr_fd("\n", 1);
		}
		token_index++;
	}
	return (data->exit_status = 0);
}
