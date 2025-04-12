/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 21:42:31 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

pid_t	execute_first_command(t_token **tokens, t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	if (pipe(cmd->pipe_fd) == -1)
		return (perror("pipe"), -1);
	parent_ignore_signals();
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
		(close(cmd->pipe_fd[0]), close(cmd->pipe_fd[1]), apply_redirections(cmd,
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
	parent_ignore_signals();
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
			apply_redirections(cmd, data), execute_for_one(data->tokens,
				current, data), close(STDIN_FILENO), close(STDOUT_FILENO),
			close_fd(cmd), free_all(data, data->tokens, cmd),
			exit(data->exit_status));
	}
	return (close(current->pipe_fd[0]), close(new_pipe_fd[1]), (pid));
}

pid_t	execute_last_command(t_token **tokens, t_cmd *current, t_cmd *cmd,
		t_data *data)
{
	pid_t	pid;

	parent_ignore_signals();
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
		(apply_redirections(cmd, data), execute_for_one(tokens, current, data),
			close(STDIN_FILENO), close(STDOUT_FILENO), close_fd(cmd),
			free_all(data, tokens, cmd), exit(data->exit_status));
	}
	if (current->pipe_fd[0] != -1)
		close(current->pipe_fd[0]);
	return (pid);
}
