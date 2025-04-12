/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 21:39:19 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

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
			if (g_signal_flag == SIGINT)
				return (data->exit_status = 130, false);
			else if (fd < 0)
				return (false);
			else
				tmp->heredoc_fd = fd;
			i++;
		}
		tmp = tmp->next;
	}
	set_signals_main();
	return (true);
}

