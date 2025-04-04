/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/04 18:00:37 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Ambigious redirection
// export a="a b"
// > $a (bash: $a: ambiguous redirect)

// export a="a b"
// echo > $a | echo > $a | echo > $a | echo hi

// > $notexists
// bash: $notexists: ambiguous redirect

// echo <"./test_files/infile_big" | cat <"./test_files/infile" (18)
// cat <missing | cat <"./test_files/infile" (38)

// cat <minishell.h|ls (118) - parsing

// cd '/////' >/dev/null (=cd /)
// cd "doesntexist" >/dev/null
// cd "wtf" >/dev/null

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
		// data->is_child = true;
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		redir_loop(cmd, data->input, data);
		//printf("I'm in child\n");
		execute_for_one(tokens, cmd, data);
		//del below
		//printf("before exit - %d\n", data->exit_status);
		(close(STDIN_FILENO), close(STDOUT_FILENO));
		(free_all(data, tokens, cmd), exit(data->exit_status));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		parent_restore_signals(); // need to move to signals
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
	int i;

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