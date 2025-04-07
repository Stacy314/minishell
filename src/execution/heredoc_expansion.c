/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/03 21:42:10 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_quoted(const char *str)
{
	size_t	len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') || (str[0] == '"'
				&& str[len - 1] == '"')))
		return (true);
	return (false);
}

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
	int		i;
	int		k;
	char	*var_value;
	char	buffer[4096];

	if (!line)
		return (NULL);
	i = 0;
	k = 0;
	while (line[i] && k < 4095)
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) || line[i
				+ 1] == '_'))
		{
			var_value = get_env(&line[i + 1], data);
			if (!var_value)
				return (NULL);
			ft_strlcpy(&buffer[k], var_value, sizeof(buffer) - k);
			k += ft_strlen(var_value);
			free(var_value);
			i++;
			while (ft_isalnum(line[i]) || line[i] == '_')
				i++;
		}
		else
			buffer[k++] = line[i++];
	}
	buffer[k] = '\0';
	result = ft_strdup(buffer);
	return (result);
}
