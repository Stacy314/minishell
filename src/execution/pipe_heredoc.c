/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 22:29:20 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	close_fd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->heredoc_fd > 0)
		{
			close(tmp->heredoc_fd);
			tmp->heredoc_fd = -1;
		}
		tmp = tmp->next;
	}
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
	(close(write_fd), free_all(data, data->tokens, data->cmd), 
	exit(0));
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

int	handle_all_heredocs(t_cmd *cmd, t_data *data)
{
	t_cmd	*tmp;
	int		fd;
	int		i;

	tmp = cmd;
	while (tmp)
	{
		i = 0;
		while (tmp->heredoc_delimiter && tmp->heredoc_delimiter[i])
		{
			fd = handle_heredoc(tmp, tmp->heredoc_delimiter[i], 128, data);
			if (fd == -2)
			{
				data->exit_status = 130;
				return (set_signals_main(), false);
			}
			else if (fd < 0)
				return (false);
			else
				tmp->heredoc_fd = fd;
			i++;
		}
		tmp = tmp->next;
	}
	return (true);
}

void	apply_redirections_for_heredoc(t_cmd *cmd, t_data *data)
{
	// int	i;
	if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			exit(1);
		}
		close(cmd->heredoc_fd);
	}
	apply_redirections(cmd, data);
	// i = 0;
	// while (data->input[i])
	//{
	//	if (data->input[i] == '>' && data->input[i + 1] == '>')
	//		handle_append_redirect(data, cmd);
	//	else if (data->input[i] == '<' && data->input[i + 1] == '<')
	//		;
	//	else if (data->input[i] == '<')
	//		handle_input_redirect(data, cmd);
	//	else if (data->input[i] == '>')
	//		handle_output_redirect(data, cmd);
	//	i++;
	//}
}
