/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 18:34:13 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

	if (pipe(cmd->pipe_fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		data->is_child = true;
		if (dup2(cmd->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			free_all(data, tokens, cmd);
			exit(1);
		}
		close(cmd->pipe_fd[0]);
		close(cmd->pipe_fd[1]);
		apply_redirections(cmd, data);
		execute_for_one(tokens, cmd, data);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_all(data, tokens, cmd);
		exit(data->exit_status);
	}
	close(cmd->pipe_fd[1]);
	return (pid);
}

pid_t	execute_middle_command(t_token **tokens, t_cmd *current, t_cmd *cmd,
		t_data *data, int new_pipe_fd[2])
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
		data->is_child = true;
		if (dup2(current->pipe_fd[0], STDIN_FILENO) == -1)
		{
			free_all(data, tokens, current);
			return (perror("dup2 current->pipe_fd[0]"), exit(1), -1);
		}
		if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
		{
			free_all(data, tokens, current);
			return (perror("dup2 new_pipe_fd[1]"), exit(1), -1);
		}
		close(current->pipe_fd[0]);
		close(current->pipe_fd[1]);
		close(new_pipe_fd[0]);
		close(new_pipe_fd[1]);
		apply_redirections(current, data);
		execute_for_one(tokens, current, data);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_all(data, tokens, cmd);
		exit(data->exit_status);
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
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		data->is_child = true;
		if (current->pipe_fd[0] != -1)
		{
			if (dup2(current->pipe_fd[0], STDIN_FILENO) == -1)
			{
				free_all(data, tokens, current);
				return (perror("dup2 current->pipe_fd[0]"), exit(1), -1);
			}
			close(current->pipe_fd[0]);
		}
		apply_redirections(current, data);
		execute_for_one(tokens, current, data);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_all(data, tokens, cmd);
		exit(data->exit_status);
	}
	if (current->pipe_fd[0] != -1)
		close(current->pipe_fd[0]);
	return (pid);
}

int	handle_heredoc_pipe(t_cmd *cmd)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	char	*line;
	int		sig;
	int		code;

	if (pipe(pipe_fd) == -1)
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				write_error("minishell: warning: here-document at line 8 delimited by end-of-file (wanted `%s')\n",
					*cmd->heredoc_delimiter);
				break ;
			}
			if (ft_strcmp(line, *cmd->heredoc_delimiter) == 0)
			{
				free(line);
				break ;
			}
			write(pipe_fd[1], line, ft_strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			close(pipe_fd[0]);
			return (-2);
		}
	}
	else if (WIFEXITED(status))
	{
		code = WEXITSTATUS(status);
		(void)code;
	}
	return (pipe_fd[0]);
}

bool	handle_all_heredocs(t_cmd *cmd, t_data *data)
{
	t_cmd	*tmp;
	int		fd;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->heredoc_delimiter)
		{
			fd = handle_heredoc_pipe(tmp);
			if (fd == -2)
			{
				data->exit_status = 130;
				return (false);
			}
			else if (fd < 0)
				return (false);
			tmp->heredoc_fd = fd;
		}
		tmp = tmp->next;
	}
	return (true);
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
		cmd->pipe_pids[process_count++] = execute_middle_command(tokens,
				current, cmd, data, new_pipe_fd);
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
	{
		close(temp_fd[1]);
		close(temp_fd[0]);
	}
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
}
