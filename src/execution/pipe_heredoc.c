/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 23:54:19 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	close_fd(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->heredoc_fd > 0)
		{
			close(tmp->heredoc_fd);
			tmp->heredoc_fd = -1;
		}
		tmp = tmp->next;
	}
}

int	handle_all_heredocs(t_cmd *cmd, t_data *data)
{
	t_cmd	*tmp;
	int		fd;
	int		i;

	tmp = cmd;
	while (tmp)
	{
		i = 0;
		while (tmp->heredoc_delimiter && tmp->heredoc_delimiter[i])
		{
			fd = handle_heredoc(tmp, tmp->heredoc_delimiter[i], 128, data);
			if (fd == -2)
			{
				data->exit_status = 130;
				return (set_signals_main(), false);
			}
			else if (fd < 0)
				return (false);
			else
				tmp->heredoc_fd = fd;
			i++;
		}
		tmp = tmp->next;
	}
	return (true);
}

void	apply_redirections_for_heredoc(t_cmd *cmd, t_data *data)
{
	if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			exit(1);
		}
		close(cmd->heredoc_fd);
	}
	apply_redirections(cmd, data);
}
