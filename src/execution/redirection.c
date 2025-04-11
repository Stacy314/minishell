/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 17:17:16 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	redir_loop(t_cmd *cmd, const char *input, t_data *data)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '<' && input[i + 1] == '<' && cmd->heredoc_delimiter)
		{
			execute_heredoc(cmd, data);
			break ;
		}
		i++;
	}
	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && input[i + 1] == '>' && cmd->append_redirects)
		{
			handle_append_redirect(data, cmd);
			i += 2;
		}
		else if (input[i] == '<' && input[i + 1] != '<' && cmd->input_redirects)
		{
			handle_input_redirect(data, cmd);
			i++;
		}
		else if (input[i] == '>' && input[i + 1] != '>'
			&& cmd->output_redirects)
		{
			handle_output_redirect(data, cmd);
			i++;
		}
		else
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
		redir_loop(cmd, data->input, data);
		apply_redirections(cmd, data);
		execute_for_one(tokens, cmd, data);
		(close(STDIN_FILENO), close(STDOUT_FILENO));
		(free_all(data, tokens, cmd), exit(data->exit_status));
	}
	waitpid(pid, &status, 0);
	parent_restore_signals();
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			write(1, "\n", 1);
			data->exit_status = 130;
		}
	}
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	set_signals_main();
	return (1);
}

void	apply_redirections(t_cmd *cmd, t_data *data)
{
	int	i;

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
