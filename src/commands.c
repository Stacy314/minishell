/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/17 19:41:37 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ls | wc -l
// unset path

//"" (: command not found, EC - 127) (11)
// touch "" (touch: cannot touch '': No such file or directory) (13)

// awk 'BEGIN{for(i=1;i<=10;i++){for(j=1;j<=10;j++){printf("%4d ",i*j)} printf("\n")}}'
/// dev/null | tail -n 10 (36)
// awk 'BEGIN{srand(42); for(i=1;i<=1000000;i++)print int(rand()*1000)}' | awk '{sum+=$1} END {print sum/NR}' (38)

//"."

// unset PATH
// change to /bin/ - inside this directory the standard commands should work

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

static int	fork_and_exec(const char *executable, char **args, char **env,
		t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (data->exit_status = 1);
	}
	if (pid == 0)
	{
		set_child_signals(); //
		execve(executable, args, env);
		// perror("execve");
		exit(0); // 127 ?
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else
			data->exit_status = 1;
	}
	return (data->exit_status);
}

static int	execute_direct_path(char *cmd, t_data *data, char **args,
		char **env)
{
	int	error_code;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		error_code = check_permissions(cmd);
		if (error_code)
			return (error_code);
		return (fork_and_exec(cmd, args, env, data));
	}
	return (-1);
}

static int	execute_via_path(char *cmd, t_data *data, char **args, char **env)
{
	char	*path;
	char	**paths;
	char	*executable;
	int		i;

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
	fork_and_exec(executable, args, env, data);
	free(executable);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	return (free(paths), data->exit_status);
}

int	execute_command(char *cmd, t_data *data, char **args, char **env)
{
	int	result;

	result = execute_direct_path(cmd, data, args, env);
	if (result >= 0)
		return (result);
	return (execute_via_path(cmd, data, args, env));
}
