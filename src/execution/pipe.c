/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/31 15:37:28 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// need check all fd

// exit | exit | exit (shouldn't exit and shouldn't print anything)

/// bin/env | grep "_=" (32)

/// bin/env | grep "SHLVL" (34)

// echo hi | echo hi | (should open input)

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

pid_t	execute_first_command(t_token **tokens, t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	if (pipe(cmd->pipe_fd) == -1) // dont need if have one command
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		if (dup2(cmd->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			free_tokens(tokens);
			free_cmd(cmd);
			free_array(data->env);
			// close(STDOUT_FILENO);
			exit(1);
		}
		close(cmd->pipe_fd[0]);
		close(cmd->pipe_fd[1]);
		apply_redirections(cmd, data);
		execute_for_one(tokens, cmd, data);
		free_tokens(tokens);
		free_cmd(cmd);
		free_array(data->env);
		// close(STDOUT_FILENO);
		exit(data->exit_status);
	}
	close(cmd->pipe_fd[1]);
	return (pid);
}

pid_t	execute_middle_command(t_token **tokens, t_cmd *cmd, t_data *data,
		int new_pipe_fd[2])
{
	pid_t	pid;

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
		if (dup2(cmd->pipe_fd[0], STDIN_FILENO) == -1)
		{
			free_tokens(tokens);
			free_cmd(cmd);
			free_array(data->env);
			return (perror("dup2 cmd->pipe_fd[0]"), exit(1), -1);
		}
		if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
		{
			free_tokens(tokens);
			free_cmd(cmd);
			free_array(data->env);			
			return (perror("dup2 new_pipe_fd[1]"), exit(1), -1);
		}
		free_tokens(tokens);
		free_cmd(cmd);
		free_array(data->env);
		(close(cmd->pipe_fd[0]), close(new_pipe_fd[0]), close(new_pipe_fd[1]),
			apply_redirections(cmd, data), execute_for_one(tokens, cmd, data),
			exit(data->exit_status));
	}
	return (close(new_pipe_fd[1]), (pid));
}

pid_t	execute_last_command(t_token **tokens, t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (cmd->pipe_fd[0] != -1)
		{
			if (dup2(cmd->pipe_fd[0], STDIN_FILENO) == -1)
			{
				free_tokens(tokens);
				free_cmd(cmd);
				free_array(data->env);			
				return (perror("dup2 cmd->pipe_fd[0]"), exit(1), -1);
			}
			close(cmd->pipe_fd[0]);
		}
		close(cmd->pipe_fd[0]);
		apply_redirections(cmd, data);
		execute_for_one(tokens, cmd, data);
		close(0); //
		free_tokens(tokens);
		free_cmd(cmd);
		free_array(data->env);
		exit(data->exit_status);
	}
	if (cmd->pipe_fd[0] != -1)
		close(cmd->pipe_fd[0]);
	return (pid);
}

static int	handle_all_heredocs(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	tmp = cmd;
	if (!tmp->heredoc_delimiter)
		return (SUCCESS);
	while (tmp)
	{
		i = 0;
		while (tmp->heredoc_delimiter[i])
		{
			if (tmp->heredoc_delimiter[i])
			{
				tmp->heredoc_fd = handle_heredoc(tmp, tmp->heredoc_delimiter[i],
						128);
				if (tmp->heredoc_fd == -1)
				{
					perror("heredoc");
					return (ERROR);
				}
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int	launch_all_processes(t_token **tokens, t_cmd *cmd, t_data *data)
{
	t_cmd	*current;
	int		new_pipe_fd[2];
	int		temp_fd;
	int		process_count;

	current = cmd;
	process_count = 0;
	cmd->pipe_pids[process_count++] = execute_first_command(tokens, current,
			data);
	temp_fd = cmd->pipe_fd[0];
	current = current->next;
	while (current && current->next)
	{
		current->pipe_fd[0] = temp_fd;
		cmd->pipe_pids[process_count++] = execute_middle_command(tokens,
				current, data, new_pipe_fd);
		temp_fd = new_pipe_fd[0];
		current = current->next;
	}
	if (current)
	{
		current->pipe_fd[0] = temp_fd;
		cmd->pipe_pids[process_count++] = execute_last_command(tokens, current,
				data);
	}
	else
		close(temp_fd);
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
	if (!handle_all_heredocs(cmd))
		return ;
	process_count = launch_all_processes(tokens, cmd, data);
	i = 0;
	while (i < process_count)
	{
		waitpid(cmd->pipe_pids[i], &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		i++;
	}
}
