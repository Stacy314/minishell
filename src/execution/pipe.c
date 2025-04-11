/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 12:39:10 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

pid_t	execute_first_command(t_token **tokens, t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	if (pipe(cmd->pipe_fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	parent_ignore_signals();
	if (pid == 0)
	{
		set_signals_child();
		data->is_child = true;
		if (dup2(cmd->pipe_fd[1], STDOUT_FILENO) == -1)
			(perror("dup2"), close_fd(cmd), free_all(data, tokens, cmd),
				exit(1));
		(close(cmd->pipe_fd[0]), close(cmd->pipe_fd[1]), apply_redirections_for_heredoc(cmd,
				data), execute_for_one(tokens, cmd, data), close(STDIN_FILENO),
			close(STDOUT_FILENO), close_fd(cmd), free_all(data, tokens, cmd),
			exit(data->exit_status));
	}
	close(cmd->pipe_fd[1]);
	return (pid);
}

pid_t	execute_middle_command(t_cmd *current, t_cmd *cmd, t_data *data,
		int new_pipe_fd[2])
{
	pid_t	pid;

	if (pipe(new_pipe_fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		data->is_child = true;
		if (dup2(current->pipe_fd[0], STDIN_FILENO) == -1)
			return (free_all(data, data->tokens, current),
				perror("dup2 current->pipe_fd[0]"), exit(1), -1);
		if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
			return (free_all(data, data->tokens, current),
				perror("dup2 new_pipe_fd[1]"), exit(1), -1);
		(close(current->pipe_fd[0]), close(current->pipe_fd[1]),
			close(new_pipe_fd[0]), close(new_pipe_fd[1]),
			apply_redirections_for_heredoc(current, data), execute_for_one(data->tokens,
				current, data), close(STDIN_FILENO), close(STDOUT_FILENO),
			close_fd(cmd), free_all(data, data->tokens, cmd),
			exit(data->exit_status));
	}
	close(current->pipe_fd[0]);
	return (close(new_pipe_fd[1]), (pid));
}

pid_t	execute_last_command(t_token **tokens, t_cmd *current, t_cmd *cmd,
		t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		data->is_child = true;
		if (current->pipe_fd[0] != -1)
		{
			if (dup2(current->pipe_fd[0], STDIN_FILENO) == -1)
				return (free_all(data, tokens, current),
					perror("dup2 current->pipe_fd[0]"), exit(1), -1);
			close(current->pipe_fd[0]);
		}
		(apply_redirections_for_heredoc(current, data), execute_for_one(tokens, current,
				data), close(STDIN_FILENO), close(STDOUT_FILENO), close_fd(cmd),
			free_all(data, tokens, cmd), exit(data->exit_status));
	}
	if (current->pipe_fd[0] != -1)
		close(current->pipe_fd[0]);
	return (pid);
}

int	launch_all_processes(t_token **tokens, t_cmd *cmd, t_data *data)
{
	t_cmd	*current;
	int		new_pipe_fd[2];
	int		temp_fd[2];
	int		process_count;

	current = cmd;
	process_count = 0;
	cmd->pipe_pids[process_count++] = execute_first_command(tokens, current,
			data);
	temp_fd[0] = cmd->pipe_fd[0];
	temp_fd[1] = cmd->pipe_fd[1];
	current = current->next;
	while (current && current->next)
	{
		current->pipe_fd[0] = temp_fd[0];
		current->pipe_fd[1] = temp_fd[1];
		cmd->pipe_pids[process_count++] = execute_middle_command(current, cmd,
				data, new_pipe_fd);
		temp_fd[0] = new_pipe_fd[0];
		temp_fd[1] = new_pipe_fd[1];
		current = current->next;
	}
	if (current)
	{
		current->pipe_fd[0] = temp_fd[0];
		current->pipe_fd[1] = temp_fd[1];
		cmd->pipe_pids[process_count++] = execute_last_command(tokens, current,
				cmd, data);
	}
	else
		(close(temp_fd[1]), close(temp_fd[0]));
	return (process_count);
}

void	execute_pipeline(t_token **tokens, t_cmd *cmd, t_data *data)
{
	int	n_cmds;
	int	process_count;
	int	status;
	int	i;

	n_cmds = count_commands(cmd);
	if (n_cmds == 0)
		return ;
	if (!handle_all_heredocs(cmd, data))
		return ;
	cmd->pipe_pids = ft_calloc(sizeof(pid_t) * n_cmds, 1);
	if (!cmd->pipe_pids)
		return (perror("calloc"));
	process_count = launch_all_processes(tokens, cmd, data);
	i = 0;
	while (i < process_count)
	{
		waitpid(cmd->pipe_pids[i], &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		i++;
	}
	close_fd(cmd);
	set_signals_main();
}
