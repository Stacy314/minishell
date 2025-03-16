/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/16 18:57:53 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// cat <<HEREDOC
// oi
// HEREDOC

// cat <<HERE <<DOC
// oi
// HERE
// time
// DOC

// cat <<HERE | ls
// oi
// HERE

// cat <<HERE
// $USER
// oi
// HERE

// cat <minishell.h <<HERE <missing | ls
// HERE

// cat <minishell.h <<HERE | cat
// HERE

// cat <minishell.h <<HERE <missing <<DOC | echo oi
// HERE
// DOC

// cat << $
// oi
// $

// << echo oi
// echo


// delimiter should be $USER
// 	cat << $USER
// delimiter EOF (write f.e. $USER into the heredoc - should be expanded)
//	 cat << EOF
// delimiter EOF (write f.e. $USER into the heredoc - should not be expanded)
//	 cat << EOF""
// delimiter is "a c" without quotes
//	 cat << "a c"
// check with "EOF " and " EOF" when in heredoc without quotes but with the spaces - shouldn't quit - quit only with "EOF"
// 	cat << EOF
// press ctrl + d inside heredoc - should quit heredoc with a warning but not minishell
//	 cat << EOF
// 	cat << EOF
// check f.e. with ctrl + c if all heredocs are closed with one command
// 	<< 1 | << 2 | << 3 cat


void	handle_heredoc(t_cmd *cmd) // <<
{
	int pipe_fd[2];
	char *line;
	pid_t pid;

	printf("heredoc\n");

	if (pipe(pipe_fd) == -1)
	{
		//perror("pipe");
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
		set_heredoc_signals();
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
