/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 16:58:11 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	find_env_var(char **env, const char *var)
{
	int	len;
	int	i;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	builtin_unset(t_cmd *cmd, t_data *data)
{
	int	i;
	int	j;
	int	var_index;

	if (!cmd->args[1])
	{
		// printf("minishell: unset: not enough arguments\n");
		return (1);
	}
	i = 1;
	while (cmd->args[i])
	{
		var_index = find_env_var(data->env, cmd->args[i]);
		if (var_index == -1)
			return (1);
		// free(data->env[var_index]);
		j = var_index;
		while (data->env[j])
		{
			data->env[j] = data->env[j + 1];
			j++;
		}
		data->env[var_index] = NULL;
		i++;
	}
	return (0);
}
