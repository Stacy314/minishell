/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/25 22:39:41 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Ambigious redirection
// export a="a b"
// > $a (bash: $a: ambiguous redirect)

// export a="a b"
// echo > $a | echo > $a | echo > $a | echo hi

// > $notexists
// bash: $notexists: ambiguous redirect

//echo <"./test_files/infile_big" | cat <"./test_files/infile" (18)
//cat <missing | cat <"./test_files/infile" (38)

//cat <minishell.h|ls (118) - parsing



int	check_error(const char *filename)
{
	struct stat	st;

	if (stat(filename, &st) == -1)
	{
		write_error("minishell: %s: No such file or directory\n",
			filename);
		return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		write_error("minishell: %s: Permission denied\n", filename);
		return (1);
	}
	return (0);
}

void	handle_input_redirect(t_cmd *cmd) // <
{
	int fd;
	int i;
	int ret;

	if (!cmd->input_redirects)
		return ;
	i = 0;
	while (cmd->input_redirects[i])
	{
		fd = open(cmd->input_redirects[i], O_RDONLY);
		if (fd == -1)
		{
			ret = check_error(cmd->input_redirects[i]);
			if (ret != 0)
				exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		i++;
	}
	close(fd);
}

void	handle_output_redirect(t_cmd *cmd) // >
{
	int fd;
	int i;
	int ret;

	if (!cmd->output_redirects)
		return ;
	i = 0;
	while (cmd->output_redirects[i])
	{
		fd = open(cmd->output_redirects[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ret = check_error(cmd->output_redirects[i]);
			if (ret != 0)
				exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}

void	handle_append_redirect(t_cmd *cmd) // >>
{
	int fd;
	int i;
	int ret;

	if (!cmd->append_redirects)
		return ;
	i = 0;
	while (cmd->append_redirects[i])
	{
		fd = open(cmd->append_redirects[i], O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (fd == -1)
		{
			ret = check_error(cmd->append_redirects[i]);
			if (ret != 0)
				exit(1);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}

int	execute_redirection(t_cmd *cmd, t_data *data, char **env, t_token **tokens)
{
	pid_t	pid;
	int		status;
	int		i;

	(void)tokens;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		//set_child_signals();
		i = 0;
		while (data->input[i]) //need expan
		{
			if (data->input[i] == '>' && data->input[i + 1] == '>')
				handle_append_redirect(cmd);
			else if (data->input[i] == '<' && data->input[i + 1] == '<')
				handle_heredoc(cmd);
			else if (data->input[i] == '<')
				handle_input_redirect(cmd);
			else if (data->input[i] == '>')
				handle_output_redirect(cmd);
			i++;
		}
		execute_for_one(tokens, cmd, data, env);
		//execute_command(cmd->args[0], data, cmd->args, env); // unset PATH builtind should work 
		exit(data->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		///* Katya */
		//if (WEXITSTATUS(status) == 130 || WEXITSTATUS(status) == 131)
		//{
		//	if (WEXITSTATUS(status) == 131)
		//		printf("Quit (core dumped)");
		//	write (1, "\n", 1);
		//}
		///* End */
		// printf("exit status: %d\n", WEXITSTATUS(status));
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
			handle_append_redirect(cmd);
		else if (data->input[i] == '<' && data->input[i + 1] == '<')
			;
		else if (data->input[i] == '<')
			handle_input_redirect(cmd);
		else if (data->input[i] == '>')
			handle_output_redirect(cmd);
		i++;
	}
}