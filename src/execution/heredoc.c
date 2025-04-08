/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/08 16:46:16 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	make_rand_numb(void)
{
	char	cwd[256];
	int		hash;
	int		i;
	int		range;

	hash = 0;
	i = 0;
	range = HEREDOC_RAND_MAX - HEREDOC_RAND_MIN + 1;
	if (range <= 0)
		return (HEREDOC_RAND_MIN);
	if (!getcwd(cwd, sizeof(cwd)))
		return (HEREDOC_RAND_MIN);
	while (cwd[i])
		hash += cwd[i++] * 31;
	return ((hash % range) + HEREDOC_RAND_MIN);
}

static int	write_name(size_t size, int rand, char *out_filename)
{
	char		*num;
	const char	*dir = "/tmp/.minishell/";
	const char	*prefix = ".heredoc_";
	const char	*suffix = ".tmp";

	num = ft_itoa(rand);
	if (!num)
		return (-1);
	if (ft_strlen(dir) + ft_strlen(prefix) + ft_strlen(num)
		+ ft_strlen(suffix) >= size)
	{
		free(num);
		return (-1);
	}
	mkdir("/tmp/.minishell", 0700);
	ft_strlcpy(out_filename, dir, size);
	ft_strlcat(out_filename, prefix, size);
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
	char	*line;
	int		tmp_fd;
	char	*expanded;
	int		i;
	char	*tmp_filename;

	if (!heredoc_delimiter)
		return (ERROR);
	tmp_filename = malloc(size);
	if (!tmp_filename)
		return (perror("malloc"), ERROR);
	tmp_fd = create_unique_tmpfile(tmp_filename, size);
	if (tmp_fd == -1)
		return (free(tmp_filename), -1);
	i = 0;
	while (cmd->heredoc_delimiter && cmd->heredoc_delimiter[i])
	{
		if (ft_strcmp(cmd->heredoc_delimiter[i], heredoc_delimiter) == 0)
			break ;
		i++;
	}
	//set_signals_heredoc();
	while (1)
	{
		//if (g_signal_flag == SIGINT)
		//{
		//	data->exit_status = 130;
		//	g_signal_flag = 0;
		//	break ;
		//}
		line = readline("> ");
		if (!line)
		{
			write_error("minishell: warning: here-document at line 8 delimited by end-of-file (wanted `%s')\n",
				heredoc_delimiter);
			break ;
		}
		if (ft_strcmp(line, heredoc_delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (cmd->heredoc_touch_quotes && cmd->heredoc_touch_quotes[i])
		{
			write(tmp_fd, line, ft_strlen(line));
			write(tmp_fd, "\n", 1);
		}
		else
		{
			expanded = expand_heredoc(line, data);
			write(tmp_fd, expanded, ft_strlen(expanded));
			write(tmp_fd, "\n", 1);
			free(expanded);
		}
		free(line);
	}
	close(tmp_fd);
	tmp_fd = open(tmp_filename, O_RDONLY);
	unlink(tmp_filename);
	free(tmp_filename);
	return (tmp_fd);
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
