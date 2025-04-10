/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 23:30:07 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//cat Makefile > out | << lim | echo hello >> out

static void	redir_loop(t_cmd *cmd, const char *input, t_data *data)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '>' && input[i + 1] == '>' && cmd->append_redirects)
		{
			handle_append_redirect(data, cmd);
			i += 2;
			break ;
		}
		else if (input[i] == '<' && input[i + 1] == '<'
			&& cmd->heredoc_delimiter)
		{
			execute_heredoc(cmd, data);
			i += 2;
			break ;
		}
		else if (input[i] == '<' && cmd->input_redirects)
		{
			handle_input_redirect(data, cmd);
			i++;
			break ;
		}
		else if (input[i] == '>' && cmd->output_redirects)
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
		redir_loop(cmd, data->input, data);
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
	int i;

	//if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
	//{
	//	if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
	//	{
	//		perror("dup2 heredoc");
	//		exit(1);
	//	}
	//	close(cmd->heredoc_fd);
	//}
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