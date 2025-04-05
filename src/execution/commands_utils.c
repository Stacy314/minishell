/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 20:12:37 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


static size_t	count_colons(const char *path)
{
	size_t	count;

	count = 1;
	while (*path)
		if (*path++ == ':')
			count++;
	return (count);
}


char	*get_path_from_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	**split_path(const char *path)
{
	char	**paths;
	int		i;
	char	*path_copy;
	char	*token;
	int		count;

	count = count_colons(path);
	paths = ft_calloc((count + 1), sizeof(char *));
	if (!paths)
		return (NULL);
	path_copy = ft_strdup(path);
	token = ft_strtok(path_copy, ":");
	i = 0;
	while (token)
	{
		paths[i++] = ft_strdup(token);
		token = ft_strtok(NULL, ":");
	}
	paths[i] = NULL;
	free(path_copy);
	return (paths);
}

char	*find_executable(const char *cmd, char **paths)
{
	char	*path_with_slash;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		path_with_slash = ft_strjoin(paths[i], "/");
		if (!path_with_slash)
			return (NULL);
		temp = ft_strjoin(path_with_slash, cmd);
		free(path_with_slash);
		if (!temp)
			return (NULL);
		if (access(temp, X_OK) == 0)
			return (temp);
		free(temp);
		i++;
	}
	return (NULL);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*saved = NULL;
	char		*token_start;

	if (str != NULL)
		saved = str;
	if (saved == NULL)
		return (NULL);
	while (*saved && ft_strchr(delim, *saved))
		saved++;
	if (*saved == '\0')
	{
		saved = NULL;
		return (NULL);
	}
	token_start = saved;
	while (*saved && !ft_strchr(delim, *saved))
		saved++;
	if (*saved != '\0')
	{
		*saved = '\0';
		saved++;
	}
	return (token_start);
}