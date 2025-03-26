/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/26 21:11:37 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// echo test |  <<lala

// ls | cat << stop | grep "asd"
// is this good
// stop

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

// cat << lim
// "lim"
// HELLO
// 42
// lim
// testing your minishell
// limm
// lim

//<<lim cat
// "lim"
// HELLO
// 42
// lim
// testing your minishell
// limm
// lim

// cat << lim
// test
// lim

// cat << EOF
//"EOF"
//!! HELLO
//@42 !!
// EOF\t\b\n
// testing your minishell :)
// limm
// EOF

// cat << hello
//$USER
//$NOVAR
//$HOME
// hello

// cat << 'lim'
//$USER
//$NOVAR
//$HOME
// lim

// cat << lim''
//$USER
//$NOVAR
//$HOME
// lim

// cat << "lim"
//$USER
//$NOVAR
//$HOME
// lim

// cat << 'lim'
//$USER
//$NOVAR
//$HOME
// lim

// echo test |  <<lala

static int	make_rand_numb(void)
{
	char	buf[256];
	int		len;
	int		hash;
	int		offset;
	int		range;

	len = 0;
	hash = 0;
	range = HEREDOC_RAND_MAX - HEREDOC_RAND_MIN + 1;
	if (!getcwd(buf, sizeof(buf)))
		return (getpid() % range + HEREDOC_RAND_MIN);
	while (buf[len])
	{
		hash += buf[len] * (len + 1);
		len++;
	}
	offset = (getpid() ^ hash ^ (len * 31));
	return ((offset % range) + HEREDOC_RAND_MIN);
}

static int	write_name(size_t size, int rand, char *out_filename)
{
	char	*num;
	char	*prefix;
	char	*suffix;

	prefix = "/tmp/heredoc_";
	suffix = ".tmp";
	num = ft_itoa(rand);
	if (!num)
		return (-1);
	if (ft_strlen(prefix) + ft_strlen(num) + ft_strlen(suffix) >= size)
	{
		free(num);
		return (-1);
	}
	ft_strlcpy(out_filename, prefix, size);
	ft_strlcat(out_filename, num, size);
	ft_strlcat(out_filename, suffix, size);
	free(num);
	return (1);
}

static int	create_unique_tmpfile(char *out_filename, size_t size)
{
	int	tries;
	int	fd;
	int	rand;

	tries = 0;
	while (tries < 100)
	{
		rand = make_rand_numb();
		if (write_name(size, rand, out_filename) == -1)
			return (-1);
		fd = open(out_filename, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			return (fd);
		else
		{
			perror("open");
			return (-1);
		}
		tries++;
	}
	write_error("Failed to create unique heredoc tmp file\n");
	return (-1);
}

int	handle_heredoc(t_cmd *cmd, size_t size)
{
	char	tmp_filename[128];
	char	*line;
	int		tmp_fd;
	int		infile_fd;

	tmp_fd = create_unique_tmpfile(tmp_filename, size);
	if (tmp_fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || (cmd->heredoc_delimiter && ft_strcmp(line,
					*cmd->heredoc_delimiter) == 0))
		{
			free(line);
			break ;
		}
		(write(tmp_fd, line, ft_strlen(line)), write(tmp_fd, "\n", 1),
			free(line));
	}
	close(tmp_fd);
	infile_fd = open(tmp_filename, O_RDONLY);
	if (infile_fd == -1)
		return (perror("open heredoc file"), unlink(tmp_filename), -1);
	return (unlink(tmp_filename), infile_fd);
}

void	execute_heredoc(t_cmd *cmd)
{
	int	infile_fd;

	infile_fd = handle_heredoc(cmd, 128);
	if (infile_fd == -1)
		return ;
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(infile_fd);
}

int	heredoc_with_pipe(t_cmd *cmd)
{
	int	infile_fd;

	infile_fd = handle_heredoc(cmd, 128);
	return (infile_fd);
}
