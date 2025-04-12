/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 22:03:01 by apechkov         ###   ########.fr       */
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
		(ft_strlcpy(out_filename, prefix, size), ft_strlcat(out_filename, num,
				size), ft_strlcat(out_filename, suffix, size), free(num));
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
	int		tmp_fd;
	char	*tmp_filename;
	int		status;

	if (!heredoc_delimiter)
		return (ERROR);
	tmp_filename = ft_calloc((size), 1);
	tmp_fd = create_unique_tmpfile(tmp_filename, size);
	if (tmp_fd == -1)
		return (free(tmp_filename), -1);
	status = process_heredoc_input(cmd, heredoc_delimiter, tmp_fd, data);
	if (status == 0)
		unlink(tmp_filename);
	return (cleanup_heredoc_file(tmp_fd, tmp_filename, data, status));
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
