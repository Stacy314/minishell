/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/09 20:53:44 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	create_unique_tmpfile(char *out_filename, size_t size)
{
	const char	*prefix = ".heredoc_";
	const char	*suffix = ".tmp";
	static int	counter = 0;
	int			fd;
	char		*num;

	while (counter < 100000)
	{
		num = ft_itoa(counter);
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
		fd = open(out_filename, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
			return (fd);
		counter++;
	}
	write_error("Failed to create unique heredoc tmp file\n");
	return (-1);
}
int	handle_heredoc(t_cmd *cmd, char *heredoc_delimiter, size_t size,
					t_data *data)
{
	char *line;
	int tmp_fd;
	char *expanded;
	int i;
	char *tmp_filename;

	if (!heredoc_delimiter)
		return (ERROR);
	tmp_filename = ft_calloc((size), 1);
	if (!tmp_filename)
		return (perror("calloc"), ERROR);
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

	signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_IGN);

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_error("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
				heredoc_delimiter);
			unlink(tmp_filename);
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
