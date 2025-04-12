/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 19:47:14 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_permissions(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
	{
		write_error("minishell: %s: No such file or directory\n", cmd);
		return (127);
	}
	if (S_ISDIR(buf.st_mode))
	{
		write_error("minishell: %s: Is a directory\n", cmd);
		return (126);
	}
	if (access(cmd, X_OK) == -1)
	{
		write_error("minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	return (0);
}

int	fork_and_exec(char *executable, char **args, t_data *data, char **paths)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), data->exit_status = 1);
	if (pid == 0)
		handle_child_process(executable, args, data, paths);
	waitpid(pid, &status, 0);
	parent_restore_signals();
	handle_parent_status(status, data);
	set_signals_main();
	return (data->exit_status);
}

static int	execute_direct_path(char *cmd, t_data *data, char **args)
{
	int	error_code;

	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		write_error("minishell: %s: filename argument required\n", cmd);
		return (data->exit_status = 2);
	}
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		error_code = check_permissions(cmd);
		if (error_code)
			return (data->exit_status = error_code);
		return (fork_and_exec(cmd, args, data, NULL));
	}
	return (-1);
}

static int	execute_via_path(char *cmd, t_data *data, char **args)
{
	char	*path;
	char	**paths;
	char	*executable;

	path = get_path_from_env(data->env);
	if (!path)
		return (write_error("%s: command not found\n", cmd),
			data->exit_status = 127);
	paths = split_path(path);
	if (!paths)
	{
		return (data->exit_status = 1);
	}
	executable = find_executable(cmd, paths);
	if (!executable)
	{
		write_error("%s: command not found\n", cmd);
		return (free_array(paths), data->exit_status = 127);
	}
	fork_and_exec(executable, args, data, paths);
	return (free_array(paths), free(executable), data->exit_status);
}

int	execute_command(char *cmd, t_data *data, char **args)
{
	int	result;

	if (!cmd || !args)
		return (data->exit_status = 1);
	result = execute_direct_path(cmd, data, args);
	if (result >= 0)
		return (result);
	return (execute_via_path(cmd, data, args));
}
