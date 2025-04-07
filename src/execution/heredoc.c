/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 15:50:28 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	fd = -1;
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
	return (fd);
}

int	handle_heredoc(t_cmd *cmd, char *heredoc_delimiter, size_t size,
		t_data *data)
{
	char	tmp_filename[128];
	char	*line;
	int		expand;
	int		tmp_fd;
	char	*expanded;

	(void)cmd;
	if (!heredoc_delimiter)
		return (ERROR);
	tmp_fd = create_unique_tmpfile(tmp_filename, size);
	if (tmp_fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_error("minishell: warning: here-document at line 8 delimited by end-of-file (wanted `%s')\n",
				*cmd->heredoc_delimiter);
			break;
		}
		if (ft_strncmp(line, heredoc_delimiter, ft_strlen(heredoc_delimiter)) == 0)
		{
			free(line);
			break;
		}
		expand = is_quoted(line);
		if (!expand)
		{
			expanded = expand_heredoc(line, data); //need to change
			(write(tmp_fd, expanded, ft_strlen(expanded)), write(tmp_fd, "\n",
					1), free(expanded));
		}
		else
			(write(tmp_fd, line, ft_strlen(line)), write(tmp_fd, "\n", 1));
		free(line);
	}
	close(tmp_fd);
	tmp_fd = open(tmp_filename, O_RDONLY);
	return (unlink(tmp_filename), tmp_fd);
}

void	execute_heredoc(t_cmd *cmd, t_data *data)
{
	int	infile_fd;
	int	i;

	i = 0;
	while (cmd->heredoc_delimiter[i])
	{
		infile_fd = handle_heredoc(cmd, cmd->heredoc_delimiter[i], 128, data);
		if (infile_fd == -1)
			return ;
		i++;
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(infile_fd);
}
