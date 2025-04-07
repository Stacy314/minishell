/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 17:37:04 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

static char	*handle_minus(t_data *data)
{
	char	*oldpwd;

	oldpwd = get_env_value(data->env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		data->exit_status = 1;
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

static char	*get_home(t_data *data)
{
	char	*home;

	home = get_env_value(data->env, "HOME");
	if (!home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		data->exit_status = 1;
		return (NULL);
	}
	return (home);
}

static char	*get_cd_destination(t_cmd *cmd, t_data *data)
{
	char	*home;

	if (!cmd->args[1])
	{
		home = get_home(data);
		return (home);
	}
	if (ft_strncmp(cmd->args[1], "-", ft_strlen(cmd->args[1])) == 0)
		return (handle_minus(data));
	if (ft_strncmp(cmd->args[1], "~", 1) == 0)
	{
		home = get_home(data);
		home = ft_strjoin(home, cmd->args[1] + 1);
		return (home);
	}
	return (cmd->args[1]);
}

int	builtin_cd(t_cmd *cmd, t_data *data, int token_index)
{
	char	*dest_path;

	if (cmd->args[token_index] && cmd->args[token_index + 1]
		&& cmd->args[token_index + 2])
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
