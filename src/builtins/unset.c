/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/18 19:55:51 by apechkov         ###   ########.fr       */
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
		return (NULL);
	ft_strlcpy(name, arg, i + 1);
	return (name);
}

int	find_env_var(char **env, const char *var)
{
	char	*arg_name;
	int		len;
	int		i;
	char	*env_name;

	arg_name = get_var_name(var);
	if (!arg_name)
		return (-1);
	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		env_name = get_var_name(env[i]);
		if (!env_name)
		{
			i++;
			continue ;
		}
		if (ft_strncmp(arg_name, env_name, len) == 0)
			return (free(env_name), free(arg_name), i);
		free(env_name);
		i++;
	}
	return (free(arg_name), -1);
}

int	builtin_unset(t_cmd *cmd, t_data *data)
{
	int	i;
	int	j;
	int	var_index;

	if (!cmd->args[1])
		return (1);
	i = 1;
	while (cmd->args[i])
	{
		var_index = find_env_var(data->env, cmd->args[i]);
		if (var_index == -1)
			return (1);
		j = var_index;
		while (data->env[j])
		{
			data->env[j] = data->env[j + 1];
			j++;
		}
		data->env[var_index] = NULL;
		i++;
	}
	return (data->exit_status = 0);
}
