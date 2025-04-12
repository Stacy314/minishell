/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:16:19 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 22:03:22 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	find_delimiter_index(t_cmd *cmd, char *heredoc_delimiter)
{
	int	i;

	i = 0;
	while (cmd->heredoc_delimiter && cmd->heredoc_delimiter[i])
	{
		if (ft_strcmp(cmd->heredoc_delimiter[i], heredoc_delimiter) == 0)
			break ;
		i++;
	}
	return (i);
}

void	write_to_heredoc(int tmp_fd, char *line, int expand, t_data *data)
{
	char	*expanded;

	if (!expand)
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
}

int	process_heredoc_input(t_cmd *cmd, char *heredoc_delimiter, int tmp_fd,
		t_data *data)
{
	char	*line;
	int		i;

	i = find_delimiter_index(cmd, heredoc_delimiter);
	(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_IGN));
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_error("minishell: warning: here-document delimited by ");
			write_error("end-of-file (wanted `%s')\n", heredoc_delimiter);
			return (0);
		}
		if (ft_strcmp(line, heredoc_delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(tmp_fd, line, !(cmd->heredoc_touch_quotes
				&& cmd->heredoc_touch_quotes[i]), data);
		free(line);
	}
	return (1);
}

int	cleanup_heredoc_file(int tmp_fd, char *tmp_filename, t_data *data,
		int status)
{
	if (status == -2 || g_signal_flag == SIGINT)
	{
		free_all(data, data->tokens, data->cmd);
		close(tmp_fd);
		unlink(tmp_filename);
		free(tmp_filename);
		return (-2);
	}
	close(tmp_fd);
	tmp_fd = open(tmp_filename, O_RDONLY);
	unlink(tmp_filename);
	free(tmp_filename);
	return (tmp_fd);
}
