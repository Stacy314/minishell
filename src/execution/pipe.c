/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 22:19:09 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	launch_first_process(t_data *data, int *temp_fd)
{
	int	pid;

	pid = execute_first_command(data->tokens, data->cmd, data);
	temp_fd[0] = data->cmd->pipe_fd[0];
	temp_fd[1] = data->cmd->pipe_fd[1];
	return (pid);
}

static int	launch_middle_process(t_cmd *current, t_data *data, int *temp_fd,
		int *new_pipe_fd)
{
	current->pipe_fd[0] = temp_fd[0];
	current->pipe_fd[1] = temp_fd[1];
	return (execute_middle_command(current, data->cmd, data, new_pipe_fd));
}

static int	launch_last_process(t_cmd *current, t_data *data, int *temp_fd)
{
	current->pipe_fd[0] = temp_fd[0];
	current->pipe_fd[1] = temp_fd[1];
	return (execute_last_command(data->tokens, current, data->cmd, data));
}

int	launch_all_processes(t_cmd *cmd, t_data *data)
{
	t_cmd	*current;
	int		new_pipe_fd[2];
	int		temp_fd[2];
	int		process_count;

	process_count = 0;
	current = cmd;
	cmd->pipe_pids[process_count++] = launch_first_process(data, temp_fd);
	current = current->next;
	while (current && current->next)
	{
		cmd->pipe_pids[process_count++] = launch_middle_process(current, data,
				temp_fd, new_pipe_fd);
		temp_fd[0] = new_pipe_fd[0];
		temp_fd[1] = new_pipe_fd[1];
		current = current->next;
	}
	if (current)
	{
		cmd->pipe_pids[process_count++] = launch_last_process(current, data,
				temp_fd);
	}
	else
		(close(temp_fd[0]), close(temp_fd[1]));
	return (process_count);
}

void	execute_pipeline(t_cmd *cmd, t_data *data)
{
	int	n_cmds;
	int	process_count;
	int	status;
	int	i;

	n_cmds = count_commands(cmd);
	if (n_cmds == 0)
		return ;
	if (!handle_all_heredocs(cmd, data))
		return ;
	cmd->pipe_pids = ft_calloc(sizeof(pid_t) * n_cmds, 1);
	if (!cmd->pipe_pids)
		return (perror("calloc"));
	process_count = launch_all_processes(cmd, data);
	i = 0;
	while (i < process_count)
	{
		waitpid(cmd->pipe_pids[i], &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		i++;
	}
	close_fd(cmd);
	set_signals_main();
}
