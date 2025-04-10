/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 19:07:03 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	update_env_var(char *arg, t_data *data, int var_index)
{
	char	*new_val;

	new_val = ft_strdup(arg);
	if (!new_val)
	{
		perror("strdup");
		data->exit_status = 1;
		return (1);
	}
	free(data->env[var_index]);
	data->env[var_index] = new_val;
	data->exit_status = 0;
	return (0);
}

static int	append_env_var(char *arg, t_data *data)
{
	int		env_size;
	char	**new_env;
	int		j;

	env_size = 0;
	while (data->env[env_size])
		env_size++;
	new_env = ft_calloc(env_size + 2, sizeof(char *));
	if (!new_env)
		return (perror("calloc"), data->exit_status = 1, 1);
	j = 0;
	while (j < env_size)
	{
		new_env[j] = data->env[j];
		j++;
	}
	new_env[env_size] = ft_strdup(arg);
	if (!new_env[env_size])
		return (perror("strdup"), data->exit_status = 1, 1);
	new_env[env_size + 1] = NULL;
	free(data->env);
	data->env = new_env;
	data->exit_status = 0;
	return (0);
}

int	add_or_update_env(char *arg, t_data *data)
{
	int	var_index;

	var_index = find_env_var(data->env, arg);
	if (var_index != -1)
		return (update_env_var(arg, data, var_index));
	else
		return (append_env_var(arg, data));
}

static int	export_env(char *arg, t_data *data)
{
	if (!is_valid_identifier(arg))
	{
		write_error("minishell: export: `%s': not a valid identifier\n", arg);
		data->exit_status = 1;
		return (1);
	}
	return (add_or_update_env(arg, data));
}

int	builtin_export(t_cmd *cmd, t_data *data, int token_index)
{
	if (!cmd->args[token_index + 1])
	{
		print_sorted_env(data->env);
		data->exit_status = 0;
		return (0);
	}
	return (export_env(cmd->args[token_index + 1], data));
}
