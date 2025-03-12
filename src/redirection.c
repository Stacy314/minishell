/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/10 20:33:37 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//grep hi "<infile" <         ./test_files/infile

//echo <"./test_files/infile_big" | cat <"./test_files/infile"

void	handle_input_redirect(t_cmd *cmd) // <
{
	int fd;
	int i;

	if (!cmd->input_redirects)
		return ;

	i = 0;
	while (cmd->input_redirects[i])
	{
		fd = open(cmd->input_redirects[i], O_RDONLY);
		if (fd == -1)
		{
			write_error("minishell: %s: No such file or directory\n",
				cmd->input_redirects[i]);
			exit(1);
			
		}
		// else
		// {
			dup2(fd, STDIN_FILENO);
			close(fd);
		// }
		i++;
	}
	close(fd);
}

void	handle_output_redirect(t_cmd *cmd) // >
{
	int fd;
	int i;

	if (!cmd->output_redirects)
		return ;

	i = 0;
	while (cmd->output_redirects[i])
	{
		fd = open(cmd->output_redirects[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			write_error("minishell: %s: No such file or directory\n",
				cmd->output_redirects[i]);
			exit(1);
		}
		// else
		// {
			dup2(fd, STDOUT_FILENO);
			close(fd);
		// }
		i++;
	}
}

void	handle_append_redirect(t_cmd *cmd) // >>
{
	int fd;
	int i;

	if (!cmd->append_redirects)
		return ;

	i = 0;
	while (cmd->append_redirects[i])
	{
		fd = open(cmd->append_redirects[i], O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (fd == -1)
		{
			write_error("minishell: %s: No such file or directory\n",
				cmd->append_redirects[i]);
			exit(1);
		}
		// else
		// {
			dup2(fd, STDOUT_FILENO);
			close(fd);
		// }
		i++;
	}
}

void	handle_heredoc(t_cmd *cmd) // <<
{
	int pipe_fd[2];
	char *line;
	pid_t pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strncmp(line, cmd->heredoc_delimiter, ft_strlen(line)) == 0)
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
	waitpid(pid, NULL, 0);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}

int	execute_redirection(t_cmd *cmd, t_data *data, char **env)
{
	pid_t	pid;
	int		status;

	//(void)env;
	//(void)data;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		if (cmd->heredoc_delimiter)
			handle_heredoc(cmd);
		if (cmd->input_redirects)
			handle_input_redirect(cmd);
		if (cmd->output_redirects)
			handle_output_redirect(cmd);
		if (cmd->append_redirects)
			handle_append_redirect(cmd);
		execute_command(cmd->args[0], data, cmd->args, env);
		exit(0); //
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		//printf("exit status: %d\n", WEXITSTATUS(status));	
		data->exit_status = WEXITSTATUS(status);
	}
	//data->exit_status = 0;
	return (1);
}

void	apply_redirections(t_cmd *cmd)
{
	if (cmd->heredoc_delimiter)
		handle_heredoc(cmd);
	if (cmd->input_redirects)
		handle_input_redirect(cmd);
	if (cmd->output_redirects)
		handle_output_redirect(cmd);
	if (cmd->append_redirects)
		handle_append_redirect(cmd);
}
