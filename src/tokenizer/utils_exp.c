/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 20:27:21 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 20:28:04 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_exit_status_value(t_data *data, int *j)
{
	(*j)++;
	return (ft_itoa(data->exit_status));
}

char	*extract_var_name(const char *str, int *j)
{
	int		start;
	int		k;
	char	*var_name;

	start = *j;
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		(*j)++;
	if (*j == start)
	{
		var_name = ft_calloc(2, sizeof(char));
		if (!var_name)
			return (perror("calloc1"), NULL);
		var_name[0] = '$';
		return (var_name);
	}
	var_name = ft_calloc(*j - start + 1, sizeof(char));
	if (!var_name)
		return (perror("calloc2"), NULL);
	k = 0;
	while (start < *j)
		var_name[k++] = str[start++];
	var_name[k] = '\0';
	return (var_name);
}

char	*search_env_value(char *var_name, t_data *data)
{
	int		i;
	int		len;
	char	**env;

	env = data->env;
	i = 0;
	len = ft_strlen(var_name);
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, len) && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}
