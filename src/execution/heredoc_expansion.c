/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 22:31:44 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_env(const char *var, t_data *data)
{
	int		i;
	int		len;
	char	*name;
	char	*value;

	len = 0;
	while (var[len] && (ft_isalnum(var[len]) || var[len] == '_'))
		len++;
	name = ft_substr(var, 0, len);
	if (!name)
		return (NULL);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name, len) == 0
			&& data->env[i][len] == '=')
		{
			value = ft_strdup(data->env[i] + len + 1);
			free(name);
			return (value);
		}
		i++;
	}
	free(name);
	return (ft_strdup(""));
}

char	*expand_heredoc(const char *line, t_data *data)
{
	char	*result;
	char	*var_value;
	size_t	buffer_size;
	size_t	i;
	size_t	k;
	char	*tmp;

	buffer_size = 128;
	i = 0;
	k = 0;
	if (!line)
		return (NULL);
	result = ft_calloc(buffer_size, sizeof(char));
	if (!result)
		return (perror("calloc"), NULL);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1])
				|| line[i + 1] == '_'))
		{
			var_value = get_env(&line[i + 1], data);
			if (!var_value)
				return (free(result), NULL);
			while (k + ft_strlen(var_value) >= buffer_size - 1)
			{
				tmp = ft_calloc(buffer_size * 2, sizeof(char));
				if (!tmp)
					return (free(result), free(var_value), perror("calloc"),
						NULL);
				ft_strlcpy(tmp, result, buffer_size * 2);
				free(result);
				result = tmp;
				buffer_size *= 2;
			}
			ft_strlcpy(&result[k], var_value, buffer_size - k);
			k += ft_strlen(var_value);
			free(var_value);
			i++;
			while (ft_isalnum(line[i]) || line[i] == '_')
				i++;
		}
		else
		{
			if (k + 2 >= buffer_size)
			{
				tmp = ft_calloc(buffer_size * 2, sizeof(char));
				if (!tmp)
					return (free(result), perror("calloc"), NULL);
				ft_strlcpy(tmp, result, buffer_size * 2);
				free(result);
				result = tmp;
				buffer_size *= 2;
			}
			result[k++] = line[i++];
		}
	}
	result[k] = '\0';
	return (result);
}
