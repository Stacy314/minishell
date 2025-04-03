/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/02 21:57:29 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*join_key_value(const char *key, const char *value)
{
	char	*tmp;
	char	*res;

	if (!key || !value)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, value);
	if (!res)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (res);
}

static char	**extend_envp(char **envp, char *new_entry)
{
	int		count;
	int		i;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = ft_calloc(sizeof(char *), (count + 2));
	if (!new_envp)
		return (envp);
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[count] = new_entry;
	new_envp[count + 1] = NULL;
	return (new_envp);
}

char	**set_env_value(char **envp, const char *key, const char *value)
{
	char	*new_entry;
	int		i;
	size_t	key_len;

	if (!envp || !key || !value)
		return (envp);
	new_entry = join_key_value(key, value);
	if (!new_entry)
		return (envp);
	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			break ;
		i++;
	}
	if (i >= 0)
	{
		free(envp[i]);
		envp[i] = new_entry;
		return (envp);
	}
	return (extend_envp(envp, new_entry));
}

char	*get_env_value(char **env, const char *key)
{
	int	len;
	int	i;

	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	check_cd_path(const char *dest_path, t_data *data)
{
	struct stat	path_stat;

	if (stat(dest_path, &path_stat) == -1)
	{
		write_error("minishell: cd: %s: No such file or directory\n",
			dest_path);
		data->exit_status = 1;
		return (1);
	}
	if (!S_ISDIR(path_stat.st_mode))
	{
		write_error("minishell: cd: %s: Not a directory\n", dest_path);
		data->exit_status = 1;
		return (1);
	}
	return (0);
}
