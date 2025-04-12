/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 21:33:13 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	start_heredoc(t_cmd *cmd, const char *input, t_data *data)
{
	int	i;

	data->saved_stdin = dup(STDIN_FILENO);
	if (data->saved_stdin == -1)
		perror("dup");
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
	if (cmd->input_redirects || cmd->output_redirects || cmd->append_redirects)
		execute_redirection(cmd, data, data->tokens);
	if (data->saved_stdin != -1)
		if (dup2(data->saved_stdin, STDIN_FILENO) == -1)
			(perror("restore stdin"), close(data->saved_stdin),
				close(STDIN_FILENO), close(STDOUT_FILENO));
	close(data->saved_stdin);
}

int	execute_redirection(t_cmd *cmd, t_data *data, t_token **tokens)
{
	pid_t	pid;
	int		status;
	int		sig;

	parent_ignore_signals();
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
		(apply_redirections(cmd, data), execute_for_one(tokens, cmd, data),
			(close(STDIN_FILENO), close(STDOUT_FILENO)), (free_all(data, tokens,
					cmd), exit(data->exit_status)));
	(waitpid(pid, &status, 0), set_signals_main());
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			return (write(1, "\n", 1), data->exit_status = 130,
				set_signals_main(), 1);
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
		else if (data->input[i] == '<')
			handle_input_redirect(data, cmd);
		else if (data->input[i] == '>')
			handle_output_redirect(data, cmd);
		i++;
	}
}
