/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 19:07:18 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_var_name(const char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_calloc(i + 1, 1);
	if (!name)
		return (perror("perror"), NULL);
	ft_strlcpy(name, arg, i + 1);
	return (name);
}

int	find_env_var(char **env, const char *var)
{
	char	*arg_name;
	int		i;
	char	*env_name;

	arg_name = get_var_name(var);
	if (!arg_name)
		return (-1);
	i = 0;
	while (env[i])
	{
		env_name = get_var_name(env[i]);
		if (!env_name)
		{
			i++;
			continue ;
		}
		if (ft_strcmp(arg_name, env_name) == 0)
			return (free(env_name), free(arg_name), i);
		free(env_name);
		i++;
	}
	return (free(arg_name), -1);
}

int	builtin_unset(t_cmd *cmd, t_data *data, int token_index)
{
	int	j;
	int	var_index;

	if (!cmd->args[token_index])
		return (1);
	token_index++;
	while (cmd->args[token_index])
	{
		var_index = find_env_var(data->env, cmd->args[token_index]);
		if (var_index != -1)
		{
			free(data->env[var_index]);
			j = var_index;
			while (data->env[j])
			{
				data->env[j] = data->env[j + 1];
				j++;
			}
		}
		token_index++;
	}
	return (data->exit_status = 0);
}
