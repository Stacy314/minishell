/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/27 22:29:55 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// _ value

//export a (ignore in env)

// export (19)

// export a="s -lsa"
//		l$a

// # Empty export isn't set on `env` but is set on `export`
// export hello
// env | grep hello
// export | grep hello

//echo $? (122)
//export "?"=hallo
//echo $?

//echo $? (126)
//export $?=hallo
//echo $?

//export SHLVL=max_value

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
	data->env = new_env;
	data->exit_status = 0;
	return (0);
}

static int	add_or_update_env(char *arg, t_data *data)
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
	char	*equal_sign;

	if (!is_valid_identifier(arg))
	{
		write_error("minishell: export: `%s': not a valid identifier\n", arg);
		data->exit_status = 1;
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	//if (!equal_sign)
	//{
	//	add_or_update_export(arg, data); // need to check
	//	data->exit_status = 0;
	//	return (0);
	//}
	return (add_or_update_env(arg, data));
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	if (!cmd->args[1])
	{
		print_sorted_env(data->env);
		// print_sorted_env(data->export_env);
		data->exit_status = 0;
		return (0);
	}
	return (export_env(cmd->args[1], data));
}
