/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 17:23:02 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// cd '/////' >/dev/null (STD_ERR) (48)
// cd "doesntexist" >/dev/null (bash: cd: doesntexist: No such file or directory,
// EC - 1) (54)
/// cd "wtf" >/dev/null (58)

static char	*create_env_str(const char *key, const char *value)
{
	size_t	len_key;
	size_t	len_val;
	char	*res;

	len_key = ft_strlen(key);
	len_val = ft_strlen(value);
	res = ft_calloc(len_key + 1 + len_val + 1, 1);
	if (!res)
		return (NULL);
	sprintf(res, "%s=%s", key, value); // need to change
	return (res);
}

char	**set_env_value(char **envp, const char *key, const char *value)
{
	char	*new_entry;
	size_t	key_len;
	int		count;
	char	**new_envp;
	int		i;

	if (!envp || !key || !value)
		return (envp);
	new_entry = create_env_str(key, value);
	if (!new_entry)
		return (envp);
	key_len = ft_strlen(key);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] = new_entry, envp);
		i++;
	}
	count = 0;
	while (envp[count] != NULL)
		count++;
	new_envp = ft_calloc(sizeof(char *) * (count + 2), 1);
	if (!new_envp)
		return (envp);
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	return (new_envp[count] = new_entry, new_envp[count + 1] = NULL, new_envp);
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

static void	perform_cd(const char *dest_path, t_data *data)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		old_pwd = ft_strdup("");
	if (chdir(dest_path) == -1)
	{
		perror("cd");
		free(old_pwd);
		data->exit_status = 1;
		return ;
	}
	set_env_value(data->env, "OLDPWD", old_pwd);
	free(old_pwd);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		set_env_value(data->env, "PWD", "");
		data->exit_status = 0;
		return ;
	}
	set_env_value(data->env, "PWD", new_pwd);
	free(new_pwd);
	data->exit_status = 0;
}

static int	check_cd_path(const char *dest_path, t_data *data)
{
	struct stat	path_stat;

	if (stat(dest_path, &path_stat) == -1) // change to fstat
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

static char	*get_cd_destination(t_cmd *cmd, t_data *data)
{
	char	*dest_path;
	char	*home;

	if (!cmd->args[1])
	{
		dest_path = get_env_value(data->env, "HOME");
		if (!dest_path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2),
				data->exit_status = 1, NULL);
		return (dest_path);
	}
	if (ft_strncmp(cmd->args[1], "-", ft_strlen(cmd->args[1])) == 0)
	{
		dest_path = get_env_value(data->env, "OLDPWD");
		if (!dest_path)
			return (ft_putendl_fd("minishell: cd: OLDPWD not set", 2),
				data->exit_status = 1, NULL);
		return (printf("%s\n", dest_path), dest_path);
	}
	if (ft_strncmp(cmd->args[1], "~", 1) == 0)
	{
		home = get_env_value(data->env, "HOME");
		if (!home)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2),
				data->exit_status = 1, NULL);
		return (ft_strjoin(home, cmd->args[1] + 1));
	}
	return (cmd->args[1]);
}

int	builtin_cd(t_cmd *cmd, t_data *data)
{
	char	*dest_path;

	if (cmd->args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		data->exit_status = 1;
		return (1);
	}
	dest_path = get_cd_destination(cmd, data);
	if (!dest_path)
		return (data->exit_status);
	if (check_cd_path(dest_path, data) != 0)
		return (data->exit_status);
	perform_cd(dest_path, data);
	return (data->exit_status);
}
