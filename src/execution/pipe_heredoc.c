/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 22:41:02 by apechkov         ###   ########.fr       */
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

int	handle_heredoc_pipe(t_cmd *cmd, t_data *data)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	char	*line;
	int		sig;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	parent_ignore_signals();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), close(pipe_fd[0]), close(pipe_fd[1]) - 1);
	if (pid == 0)
	{
		(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_IGN));
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				(write_error("minishell: warning: here-document delimited by "),
					write_error("end-of-file (wanted `%s')\n",
						*cmd->heredoc_delimiter));
				break ;
			}
			if (ft_strcmp(line, *cmd->heredoc_delimiter) == 0)
			{
				free(line);
				break ;
			}
			(write(pipe_fd[1], line, ft_strlen(line)), write(pipe_fd[1], "\n",
					1), free(line));
		}
		(close(pipe_fd[1]), free_all(data, data->tokens, data->cmd), exit(0));
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	parent_restore_signals();
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			data->exit_status = 130;
			close(pipe_fd[0]);
			return (-2);
		}
	}
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	set_signals_main();
	return (pipe_fd[0]);
}

int	handle_all_heredocs(t_cmd *cmd, t_data *data)
{
	t_cmd	*tmp;
	int		fd;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->heredoc_delimiter)
		{
			fd = handle_heredoc_pipe(tmp, data);
			if (fd == -2)
			{
				data->exit_status = 130;
				set_signals_main();
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
