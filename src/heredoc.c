/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/20 19:59:10 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// echo test |  <<lala

//ls | cat << stop | grep "asd"
//is this good
//stop

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
//		cat << EOF
// delimiter EOF (write f.e. $USER into the heredoc - should not be expanded)
//		cat << EOF""
// delimiter is "a c" without quotes
//		cat << "a c"
// check with "EOF " and " EOF" when in heredoc without quotes but with the spaces
	//- shouldn't quit - quit only with "EOF"
// 	cat << EOF
// press ctrl + d inside heredoc
	//- should quit heredoc with a warning but not minishell
//		cat << EOF
// 	cat << EOF
// check f.e. with ctrl + c if all heredocs are closed with one command
// 	<< 1 | << 2 | << 3 cat

//cat << lim
// "lim"
//HELLO
//42
// lim
//testing your minishell
//limm
//lim

//<<lim cat
// "lim"
//HELLO
//42
// lim
//testing your minishell
//limm
//lim

//cat << lim
//test
//lim

//cat << EOF
//"EOF"
//!! HELLO
//@42 !!
// EOF\t\b\n
//testing your minishell :)
//limm
//EOF

//cat << hello
//$USER
//$NOVAR
//$HOME
//hello

//cat << 'lim'
//$USER
//$NOVAR
//$HOME
//lim

//cat << lim''
//$USER
//$NOVAR
//$HOME
//lim

//cat << "lim"
//$USER
//$NOVAR
//$HOME
//lim

//cat << 'lim'
//$USER
//$NOVAR
//$HOME
//lim

//echo test |  <<lala


//void	handle_heredoc(t_cmd *cmd) // <<
//{
//	int pipe_fd[2];
//	char *line;
//	pid_t pid;

//	if (pipe(pipe_fd) == -1)
//	{
//		// perror("pipe");
//		exit(1);
//	}
//	pid = fork();
//	if (pid == -1)
//	{
//		perror("fork");
//		exit(1);
//	}
//	if (pid == 0)
//	{
//		//set_heredoc_signals();
//		close(pipe_fd[0]);
//		while (1)
//		{
//			line = readline("> ");
//			if (!line || ft_strncmp(line, *cmd->heredoc_delimiter,
//					ft_strlen(*cmd->heredoc_delimiter)) == 0)
//			{
//				/*printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')", *cmd->heredoc_delimiter);*/
//				free(line);
//				break ;
//			}
//			write(pipe_fd[1], line, ft_strlen(line));
//			write(pipe_fd[1], "\n", 1);
//			free(line);
//		}
//		close(pipe_fd[1]);
//		//exit(0);
//	}
//	waitpid(pid, NULL, 0);
//	close(pipe_fd[1]);
//	dup2(pipe_fd[0], STDIN_FILENO);
//	close(pipe_fd[0]);
//}


void	handle_heredoc(t_cmd *cmd)
{
	char	*line;
	char	tmp_filename[] = "/tmp/heredocXXXXXX";
	int		tmp_fd;
	int		infile_fd;

	// Створюємо тимчасовий файл
	tmp_fd = mkstemp(tmp_filename); //need to check
	if (tmp_fd == -1)
	{
		perror("mkstemp");
		return ;
	}
	while (1)
	{
		line = readline("> ");
		// Якщо Ctrl+D (line == NULL) або line == delimiter – break
		if (!line || (cmd->heredoc_delimiter
			&& ft_strncmp(line, *cmd->heredoc_delimiter, ft_strlen(*cmd->heredoc_delimiter) == 0)))
		{
			free(line);
			break ;
		}
		dprintf(tmp_fd, "%s\n", line); //need to change
		free(line);
	}
	close(tmp_fd);

	// 3) Відкриваємо цей файл ще раз на читання
	infile_fd = open(tmp_filename, O_RDONLY);
	if (infile_fd == -1)
	{
		perror("open heredoc file");
		unlink(tmp_filename);
		return ;
	}

	// 4) Dup2, щоб замінити STDIN на наш файл
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(infile_fd);

	// 5) Видаляємо тимчасовий файл, бо він нам більше не потрібен
	unlink(tmp_filename); //need to check
}
