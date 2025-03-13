/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/13 22:48:29 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// export a="s -lsa"
//		l$a
// export a='"' (shouldn't crash)
//		$a
// export a='"'
//		echo $a

/// bin/echo $HALLO42 (1)
// env | grep HALLO42
// export HALLO42=42
/// bin/echo $HALLO42
// env | grep HALLO42

/// bin/echo $HOME (2)
// env | grep HOME
// export HOME=42
/// bin/echo $HOME
// env | grep HOME

//export TES_T=123  (113)
///bin/echo $TES_T
//env | grep TES_T

//export HELLO42=T="" (117)
///bin/echo $HELLO42
//env | grep HELLO42
//env | grep HELLO42=T=

///bin/echo $mini_test (134)
//env | grep mini_test
//export mini_test=
///bin/echo $mini_test
//env | grep mini_test

//export (wrong output?) (4)

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

static int	export_one_arg(char *arg, t_data *data)
{
	char	*equal_sign;

	if (!is_valid_identifier(arg))
	{
		write_error("minishell: export: `%s': not a valid identifier\n", arg);
		data->exit_status = 1;
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		/////////
		add_or_update_export(arg, data);
		data->exit_status = 0;
		return (0); //1
	}
	return (add_or_update_env(arg, data));
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	if (!cmd->args[1])
	{
		print_sorted_env(data->env);
		//print_sorted_env(data->export_env); 
		data->exit_status = 0;
		return (0);
	}
	// if (cmd->args[2])  
	// {
	// 	write_error("minishell: export: too many arguments\n");
	// 	data->exit_status = 1;
	// 	return (1);
	// }
	return (export_one_arg(cmd->args[1], data));
}
