/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/04 14:10:08 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// "" (: command not found, EC - 127) (11)
// touch "" (touch: cannot touch '': No such file or directory) (13)

//"."

//////////////////////////////////////////////////////////////////////////////
void	debug_paths_info(const char *cmd, char **paths, const char *found_path)
{
	int	i;

	printf("Debug info for command: \"%s\"\n", cmd);
	printf("Parsed PATH directories:\n");
	for (i = 0; paths && paths[i]; i++)
		printf("  [%d]: %s\n", i, paths[i]);

	if (found_path)
		printf("Executable found at: %s\n", found_path);
	else
		printf("Executable NOT found for command: %s\n", cmd);

	printf("----------------------------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////


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

static int	fork_and_exec(const char *executable, char **args, t_data *data)
{
	pid_t pid;
	int status;
	int sig;

	parent_ignore_signals(); // move?
	if (data->is_child == false)
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), data->exit_status = 1);
		if (pid == 0)
			(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL),
				execve(executable, args, data->env), /*free_all(data,
					data->tokens, data->cmd),*/ exit(0));
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
		data->is_child = false; // maybe delete? because it is in child
		// signal(SIGINT, SIG_DFL);
		// signal(SIGQUIT, SIG_DFL);
		execve(executable, args, data->env);
		//perror("execve");
		//free_all(data, data->tokens, data->cmd);
	}
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
		return (write_error("%s: command not found\n", cmd), data->exit_status = 127);
	paths = split_path(path);
	if (!paths)
	{
		return (free_all(data,
			data->tokens, data->cmd),data->exit_status = 1);
	}
	executable = find_executable(cmd, paths);
	//debug_paths_info(cmd, paths, executable);
	if (!executable)
	{
		write_error("%s: command not found\n", cmd);
		return (free_array(paths), data->exit_status = 127);
	}
	(fork_and_exec(executable, args, data), free(executable));
	i = 0;
	while (paths[i])
		free(paths[i++]);
	return (free(paths), data->exit_status);
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
