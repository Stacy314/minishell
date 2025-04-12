/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 19:51:49 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_error(const char *filename)
{
	struct stat	st;

	if (stat(filename, &st) == -1)
	{
		write_error("minishell: %s: No such file or directory\n", filename);
		return (1);
	}
	if (access(filename, R_OK) == -1)
	{
		write_error("minishell: %s: Permission denied\n", filename);
		return (1);
	}
	if (S_ISDIR(st.st_mode))
	{
		write_error("minishell: %s: Is a directory\n", filename);
		return (1);
	}
	return (0);
}

void	handle_input_redirect(t_data *data, t_cmd *cmd)
{
	int	fd;
	int	i;
	int	ret;

	if (!cmd->input_redirects)
		return ;
	i = 0;
	while (cmd->input_redirects[i])
	{
		fd = open(cmd->input_redirects[i], O_RDONLY);
		if (fd == -1)
		{
			ret = check_error(cmd->input_redirects[i]);
			if (ret != 0)
				(close(fd), close(STDOUT_FILENO), close(STDIN_FILENO),
					free_all(data, data->tokens, data->cmd), exit(1));
		}
		if (dup2(fd, STDIN_FILENO) == -1)
			(perror("dup2"), close(STDOUT_FILENO), close(STDIN_FILENO),
				close(fd), free_all(data, data->tokens, data->cmd), exit(1));
		close(fd);
		i++;
	}
	close(fd);
}

void	handle_output_redirect(t_data *data, t_cmd *cmd)
{
	int	fd;
	int	i;

	if (!cmd->output_redirects)
		return ;
	i = 0;
	while (cmd->output_redirects[i])
	{
		fd = open(cmd->output_redirects[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			check_error(cmd->output_redirects[i]);
			(close(fd), close(STDOUT_FILENO), close(STDIN_FILENO),
				free_all(data, data->tokens, data->cmd), exit(1));
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), close(STDOUT_FILENO), close(STDIN_FILENO),
				close(fd), free_all(data, data->tokens, data->cmd), exit(1));
		close(fd);
		i++;
	}
	close(fd);
}

void	handle_append_redirect(t_data *data, t_cmd *cmd)
{
	int	fd;
	int	i;

	if (!cmd->append_redirects)
		return ;
	i = 0;
	while (cmd->append_redirects[i])
	{
		fd = open(cmd->append_redirects[i], O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (fd == -1)
		{
			check_error(cmd->append_redirects[i]);
			(close(fd), close(STDOUT_FILENO), close(STDIN_FILENO),
				free_all(data, data->tokens, data->cmd), exit(1));
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), close(STDOUT_FILENO), close(STDIN_FILENO),
				close(fd), free_all(data, data->tokens, data->cmd), exit(1));
		close(fd);
		i++;
	}
	close(fd);
}
