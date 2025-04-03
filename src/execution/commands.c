/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/02 21:23:08 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// "" (: command not found, EC - 127) (11)
// touch "" (touch: cannot touch '': No such file or directory) (13)

//"."

int	check_permissions(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
	{
		write_error("minishell: %s: No such file or directory\n", cmd);
		return (127); // 2
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

static int	fork_and_exec(const char *executable, char **args, t_data *data)
// write whithout child
{
	pid_t pid;
	int status;
	int sig;

	parent_ignore_signals();
	// if (!executable || !args)
	//	return (data->exit_status = 1);
	if (data->is_child == false)
	{
		// printf("im here\n");
		pid = fork();
		if (pid == -1)
			return (perror("fork"), data->exit_status = 1);
		if (pid == 0)
			(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL),
				execve(executable, args, data->env), free_all(data,
					data->tokens, data->cmd), exit(0));
		waitpid(pid, &status, 0);
		parent_restore_signals();
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status); // need to move to signals
			if (sig == SIGINT)
			{
				data->exit_status = 130;
				write(STDOUT_FILENO, "\n", 1);
			}
			else if (sig == SIGQUIT)
			{
				data->exit_status = 131;
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			}
			else
				data->exit_status = 128 + sig;
		}
		else if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else
			data->exit_status = 1;
	}
	else
	{
		data->is_child = false; // maybe delete? becouse it is in child
		// signal(SIGINT, SIG_DFL);
		// signal(SIGQUIT, SIG_DFL);
		// printf("im here\n");
		execve(executable, args, data->env);
		/// do we go there?
		perror("execve");
		free_all(data, data->tokens, data->cmd);
	}
	return (data->exit_status);
}

static int	execute_direct_path(char *cmd, t_data *data, char **args)
{
	int	error_code;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		error_code = check_permissions(cmd);
		if (error_code)
			return (/*printf("im here\n"), free_all(data, data->tokens,
					data->cmd),*/ data->exit_status = error_code);
		return (fork_and_exec(cmd, args, data));
	}
	return (-1);
}

static int	execute_via_path(char *cmd, t_data *data, char **args)
{
	char	*path;
	char	**paths;
	char	*executable;
	int		i;

	path = get_path_from_env(data->env);
	if (!path)
		return (write_error("%s: command not found\n", cmd),/* free_all(data,
				data->tokens, data->cmd),*/ data->exit_status = 127);
	paths = split_path(path);
	if (!paths)
	{
		return (free_all(data,
			data->tokens, data->cmd),data->exit_status = 1);
	}
	executable = find_executable(cmd, paths);
	if (!executable)
	{
		write_error("%s: command not found\n", cmd);
		return (free_array(paths),/* free_all(data,
			data->tokens, data->cmd),*/ data->exit_status = 127);
	}
	(fork_and_exec(executable, args, data), free(executable));
	i = 0;
	while (paths[i])
		free(paths[i++]);
	return (free(paths), /*free_all(data,
		data->tokens, data->cmd),*/ data->exit_status);
}

int	execute_command(char *cmd, t_data *data, char **args)
{
	int	result;

	result = execute_direct_path(cmd, data, args);
	if (result >= 0)
		return (result);
	return (execute_via_path(cmd, data, args));
}
