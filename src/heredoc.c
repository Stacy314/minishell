/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/16 17:09:37 by anastasiia       ###   ########.fr       */
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
