/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredoc2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 23:54:17 by apechkov         ###   ########.fr       */
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

static int	create_heredoc_pipe_and_fork(int pipe_fd[2], pid_t *pid)
{
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	parent_ignore_signals();
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	return (0);
}

static void	child_handle_heredoc(int write_fd, t_cmd *cmd, t_data *data)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	close(cmd->pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_error("minishell: warning: here-document delimited by ");
			write_error("end-of-file (wanted `%s')\n", *cmd->heredoc_delimiter);
			break ;
		}
		if (ft_strcmp(line, *cmd->heredoc_delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	(close(write_fd), free_all(data, data->tokens, data->cmd), exit(0));
}

static int	handle_child_exit_status(int status, t_data *data, int read_fd)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			data->exit_status = 130;
			close(read_fd);
			return (-2);
		}
	}
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	return (read_fd);
}

int	handle_heredoc_pipe(t_cmd *cmd, t_data *data)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (create_heredoc_pipe_and_fork(pipe_fd, &pid) != 0)
		return (-1);
	if (pid == 0)
		child_handle_heredoc(pipe_fd[1], cmd, data);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	parent_restore_signals();
	set_signals_main();
	return (handle_child_exit_status(status, data, pipe_fd[0]));
}
