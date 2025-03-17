/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/17 19:41:35 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_delim(char c, const char *delim)
{
	return (ft_strchr(delim, c) != NULL);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*saved = NULL;
	char		*token_start;

	if (str != NULL)
		saved = str;
	if (saved == NULL)
		return (NULL);
	while (*saved && is_delim(*saved, delim))
		saved++;
	if (*saved == '\0')
	{
		saved = NULL;
		return (NULL);
	}
	token_start = saved;
	while (*saved && !is_delim(*saved, delim))
		saved++;
	if (*saved != '\0')
	{
		*saved = '\0';
		saved++;
	}
	return (token_start);
}

char	*get_path_from_env(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			return (env[i] + 5);
		}
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

	paths = ft_calloc(128 * sizeof(char *), 1); // why 128?
	if (!paths)
	{
		// free
		return (NULL);
	}
	path_copy = ft_strdup(path);
	token = ft_strtok(path_copy, ":"); // need to cange
	i = 0;
	while (token)
	{
		paths[i++] = ft_strdup(token);
		token = ft_strtok(NULL, ":"); // need to cange
	}
	paths[i] = NULL;
	free(path_copy);
	return (paths);
}

char	*find_executable(const char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	full_path = ft_calloc(1024, 1); // why 1024?
	if (!full_path)
	{
		// free(void *ptr)
		perror("calloc");
		return (NULL);
	}
	i = 0;
	while (paths[i] != NULL)
	{
		snprintf(full_path, 1024, "%s/%s", paths[i], cmd); // need to change
		if (access(full_path, X_OK) == 0)
		{
			// printf("full_path = %s\n", full_path);
			return (full_path);
		}
		i++;
	}
	free(full_path);
	return (NULL);
}
