/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/17 19:41:11 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// env | grep DOESNT_EXIST (shouldn't print anything, I write in stderr)

// env | grep HOME

// env | grep USER

// env | wc -l

// int	builtin_env(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (data->env[i])
// 	{
// 		ft_putstr_fd(data->env[i], 1);
// 		ft_putstr_fd("\n", 1);
// 		i++;
// 	}
// 	return (data->exit_status = 0);
// }

int	builtin_env(t_data *data, t_cmd *cmd) // check if it the same as in bash
{
	int i;

	if (!data->env) //
		return (data->exit_status = 1);
	if (cmd->args[1]) //
	{
		write_error("env: too many arguments\n");
		return (data->exit_status = 1);
	}
	i = 0;
	while (data->env[i])
	{
		ft_putstr_fd(data->env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (data->exit_status = 0);
}
