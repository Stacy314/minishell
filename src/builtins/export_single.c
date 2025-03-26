/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_single.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/26 16:56:59 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// # Empty export isn't set on `env` but is set on `export`
// export hello
// env | grep hello
// export | grep hello

static int	find_in_env(char **env, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	find_in_export_env(char **exp, const char *key)
{
	int	i;

	i = 0;
	while (exp && exp[i])
	{
		if (!ft_strncmp(exp[i], key, ft_strlen(key)))
			return (i);
		i++;
	}
	return (-1);
}

static size_t	count_strings(char **arr)
{
	size_t	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static char	**append_to_str_array(char **arr, const char *str)
{
	size_t	old_size;
	char	**new_arr;
	size_t	i;

	old_size = count_strings(arr);
	i = 0;
	new_arr = ft_calloc(sizeof(char *) * (old_size + 2), 1);
	if (!new_arr)
		return (NULL);
	while (i < old_size)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = ft_strdup(str);
	if (!new_arr[i])
	{
		// free(new_arr);
		return (NULL);
	}
	i++;
	new_arr[i] = NULL;
	// free(arr);
	return (new_arr);
}

void	add_or_update_export(char *key, t_data *data)
{
	if (find_in_env(data->env, key) != -1)
		return ;
	if (find_in_export_env(data->export_env, key) != -1)
		return ;
	data->export_env = append_to_str_array(data->export_env, key);
	data->exit_status = 0;
}
