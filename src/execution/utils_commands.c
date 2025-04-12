/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:47:52 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 19:48:23 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_child_process(char *executable, char **args, t_data *data,
		char **paths)
{
	set_signals_child();
	execve(executable, args, data->env);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free_array(paths);
	free(executable);
	free_all(data, data->tokens, data->cmd);
	exit(0);
}

void	handle_parent_status(int status, t_data *data)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			data->exit_status = 130;
			write(STDOUT_FILENO, "\n", 1);
		}
		else if (sig == SIGQUIT)
		{
			data->exit_status = 131;
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		}
	}
	else if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
}
