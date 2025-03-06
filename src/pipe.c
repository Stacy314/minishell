/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 18:42:43 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// exit | exit | exit (shouldn't exit and shouldn't print anything)
// cat minishell.h | grep ");"$ (exit code - 1)
// export GHOST=123 | env | grep GHOST (exit code - 1)

char	*find_command_path(const char *cmd, char **env)
{
	char	*path_env;
	char	*paths;
	size_t	len_cmd;
	char	*token;
	char	*full_path;
	size_t	len_dir;

	path_env = get_path_from_env(env);
	if (!path_env)
		return (NULL);
	paths = ft_strdup(path_env);
	if (!paths)
	{
		perror("strdup");
		return (NULL);
	}
	len_cmd = ft_strlen(cmd);
	token = strtok(paths, ":");
	full_path = NULL;
	while (token)
	{
		len_dir = strlen(token);
		full_path = malloc(len_dir + 1 + len_cmd + 1);
		if (!full_path)
		{
			perror("malloc");
			free(paths);
			return (NULL);
		}
		ft_strlcpy(full_path, token, len_dir + 1);
		strcat(full_path, "/");
		strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		full_path = NULL;
		token = strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

pid_t	execute_first_command(t_token **tokens, t_cmd *cmd, t_data *data,
		char **env, int pipe_fd[2])
{
	pid_t	pid;

	// char	*cmd_path;
	(void)data;
	// printf("execute_first_command\n");
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	// if (strchr(cmd->args[0], '/'))
	//	cmd_path = cmd->args[0];
	// else
	//	cmd_path = find_command_path(cmd->args[0], env);
	// if (!cmd_path)
	//{
	//	fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
	//	exit(127);
	//}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		close(1);
		execute_for_one(tokens, cmd, data, env);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[1]);
	return (pid);
}

pid_t	execute_middle_command(t_token **tokens, t_cmd *cmd, t_data *data,
		char **env, int in_fd, int new_pipe_fd[2])
{
	pid_t	pid;

	// char	*cmd_path;
	(void)data;
	// printf("execute_middle_command\n");
	if (pipe(new_pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit(1);
		}
		if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 new_pipe_fd[1]");
			exit(1);
		}
		close(in_fd);
		close(new_pipe_fd[0]);
		close(new_pipe_fd[1]);
		close(0);
		close(1);
		// if (strchr(cmd->args[0], '/'))
		//	cmd_path = cmd->args[0];
		// else
		//	cmd_path = find_command_path(cmd->args[0], env);
		// if (!cmd_path)
		//{
		//	fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
		//	exit(127);
		//}
		// execve(cmd_path, cmd->args, env);
		// perror("execve");
		execute_for_one(tokens, cmd, data, env);
		exit(1);
	}
	close(in_fd);
	close(new_pipe_fd[1]);
	return (pid);
}

pid_t	execute_last_command(t_token **tokens, t_cmd *cmd, t_data *data,
		char **env, int in_fd)
{
	pid_t	pid;

	// char	*cmd_path;
	(void)data;
	// printf("execute_last_command\n");
	// if (strchr(cmd->args[0], '/'))
	//	cmd_path = cmd->args[0];
	// else
	//	cmd_path = find_command_path(cmd->args[0], env);
	// if (!cmd_path)
	//{
	//	fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
	//	exit(127);
	//}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (in_fd != -1)
		{
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 in_fd");
				exit(1);
			}
			close(in_fd);
			//close(0);
		}
		execute_for_one(tokens, cmd, data, env);
		exit(EXIT_SUCCESS);
	}
	if (in_fd != -1)
		close(in_fd);
	return (pid);
}

// void	print_cmd_list1(t_cmd *cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (cmd)
// 	{
// 		printf("=== Command %d ===\n", i);
// 		if (cmd->args)
// 		{
// 			printf("Args: ");
// 			for (int j = 0; cmd->args[j]; j++)
// 				printf("\"%s\" ", cmd->args[j]);
// 			printf("\n");
// 		}
// 		else
// 			printf("Args: (none)\n");
// 		printf("Input redirect: %s\n",
// 			cmd->input_redirect ? cmd->input_redirect : "(none)");
// 		printf("Output redirect: %s\n",
// 			cmd->output_redirect ? cmd->output_redirect : "(none)");
// 		printf("Append redirect: %s\n",
// 			cmd->append_redirect ? cmd->append_redirect : "(none)");
// 		printf("Heredoc delimiter: %s\n",
// 			cmd->heredoc_delimiter ? cmd->heredoc_delimiter : "(none)");
// 		// printf("Pipe in: %d | Pipe out: %d\n", cmd->pipe_in, cmd->pipe_out);
// 		printf("Next: %s\n", cmd->next ? "Exists" : "NULL");
// 		printf("--------------------------\n");
// 		cmd = cmd->next;
// 		i++;
// 	}
// }

void	execute_pipeline(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	int		n_cmds;
	
	int		pipe_fd[2], new_pipe_fd[2];
	int		process_count;
	t_cmd	*current;
	int		status;
	int		in_fd;
	

	// print_cmd_list1(cmd);
	n_cmds = count_commands(cmd);
	if (n_cmds == 0)
		return ;
	pid_t	pids[n_cmds]; //need to fix
	process_count = 0;
	current = cmd;
	pids[process_count++] = execute_first_command(tokens, current, data, env,
			pipe_fd);
	in_fd = pipe_fd[0];
	current = current->next;
	while (current && current->next)
	{
		pids[process_count++] = execute_middle_command(tokens, current, data,
				env, in_fd, new_pipe_fd);
		in_fd = new_pipe_fd[0];
		current = current->next;
	}
	if (current)
		pids[process_count++] = execute_last_command(tokens, current, data, env,
				in_fd);
	else
		close(in_fd);
	for (int i = 0; i < process_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
	}
}
