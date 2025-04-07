/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 17:26:45 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_error(const char *filename)
{
	char	*error;

	error = ft_strjoin("minishell: ", filename);
	if (!error)
	{
		perror("malloc");
		return (1);
	}
	perror(error);
	free(error);
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
			{
				close(fd);
				close(STDOUT_FILENO);
				close(STDIN_FILENO);
				free_all(data, data->tokens, data->cmd);
				exit(1);
			}
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		i++;
	}
	close(fd);
}

void	handle_output_redirect(t_data *data, t_cmd *cmd)
{
	int	fd;
	int	i;
	int	ret;

	if (!cmd->output_redirects)
		return ;
	i = 0;
	while (cmd->output_redirects[i])
	{
		fd = open(cmd->output_redirects[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ret = check_error(cmd->output_redirects[i]);
			if (ret != 0)
			{
				close(fd);
				close(STDOUT_FILENO);
				close(STDIN_FILENO);
				free_all(data, data->tokens, data->cmd);
				exit(1);
			}
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}

void	handle_append_redirect(t_data *data, t_cmd *cmd)
{
	int	fd;
	int	i;
	int	ret;

	(void)data;
	if (!cmd->append_redirects)
		return ;
	i = 0;
	while (cmd->append_redirects[i])
	{
		fd = open(cmd->append_redirects[i], O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (fd == -1)
		{
			ret = check_error(cmd->append_redirects[i]);
			if (ret != 0)
			{
				close(fd);
				close(STDOUT_FILENO);
				close(STDIN_FILENO);
				free_all(data, data->tokens, data->cmd);
				exit(1);
			}
			close(fd);
			close(STDOUT_FILENO);
			close(STDIN_FILENO);
			free_all(data, data->tokens, data->cmd);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}
