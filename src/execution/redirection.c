/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 17:23:45 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	redir_loop(t_cmd *cmd, const char *input, t_data *data)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && input[i + 1] == '>')
		{
			handle_append_redirect(data, cmd);
			i += 2;
			break ;
		}
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			execute_heredoc(cmd, data);
			i += 2;
			break ;
		}
		else if (input[i] == '<')
		{
			handle_input_redirect(data, cmd);
			i++;
			break ;
		}
		else if (input[i] == '>')
		{
			handle_output_redirect(data, cmd);
			i++;
			break ;
		}
		i++;
	}
}

int	execute_redirection(t_cmd *cmd, t_data *data, t_token **tokens)
{
	pid_t	pid;
	int		status;
	int		sig;

	(void)tokens;
	parent_ignore_signals();
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		redir_loop(cmd, data->input, data);
		execute_for_one(tokens, cmd, data);
		(close(STDIN_FILENO), close(STDOUT_FILENO));
		(free_all(data, tokens, cmd), exit(data->exit_status));
		exit(data->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		parent_restore_signals();
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGINT)
			{
				write(STDOUT_FILENO, "\n", 1);
				data->exit_status = 130;
			}
			else if (sig == SIGQUIT)
			{
				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				data->exit_status = 131;
			}
			else
				data->exit_status = 128 + sig;
		}
		else if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
	}
	return (1);
}

void	apply_redirections(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			exit(1);
		}
		close(cmd->heredoc_fd);
	}
	i = 0;
	while (data->input[i])
	{
		if (data->input[i] == '>' && data->input[i + 1] == '>')
			handle_append_redirect(data, cmd);
		else if (data->input[i] == '<' && data->input[i + 1] == '<')
			;
		else if (data->input[i] == '<')
			handle_input_redirect(data, cmd);
		else if (data->input[i] == '>')
			handle_output_redirect(data, cmd);
		i++;
	}
}
