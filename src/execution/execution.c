/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 22:17:10 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	executiom_first(t_token **tokens, t_cmd *cmd, t_data *data, int i)
{
	if (tokens[i])
	{
		if (ft_strncmp(cmd->args[i], "echo", 5) == 0)
			builtin_echo(cmd, data, i);
		else if (ft_strncmp(cmd->args[i], "cd", 3) == 0)
			builtin_cd(cmd, data, i);
		else if (ft_strncmp(cmd->args[i], "pwd", 4) == 0)
			builtin_pwd(cmd, data, i);
		else if (ft_strncmp(cmd->args[i], "export", 7) == 0)
			builtin_export(cmd, data, i);
		else if (ft_strncmp(cmd->args[i], "unset", 6) == 0)
			builtin_unset(cmd, data, i);
		else if (ft_strncmp(cmd->args[i], "env", 4) == 0)
			builtin_env(data, cmd, i);
		else if (ft_strncmp(cmd->args[i], "exit", 5) == 0)
			builtin_exit(cmd, data, tokens, i);
		else
			data->exit_status = execute_command(cmd->args[0], data, cmd->args);
	}
}

void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data)
{
	int	i;

	i = 0;
	if (!cmd->args || !cmd->args[0])
		return ;
	while (cmd->args[i] && tokens[i] && tokens[i]->type == NOTHING)
	{
		i++;
	}
	if (!cmd->args || !cmd->args[i])
		return ;
	if (cmd->args[i][0] == '\0' && tokens[i]->type != NOTHING)
	{
		write_error("%s: command not found\n", cmd->args[i]);
		data->exit_status = 127;
		return ;
	}
	executiom_first(tokens, cmd, data, i);
}

void	execute(t_token **tokens, t_cmd *cmd, t_data *data)
{
	if (contains_special_char(tokens, PIPE))
	{
		execute_pipeline(cmd, data);
		return ;
	}
	else if (cmd->heredoc_delimiter || cmd->input_redirects
		|| cmd->output_redirects || cmd->append_redirects)
	{
		if (execute_redirection(cmd, data, tokens) == 1)
			return ;
	}
	else
		execute_for_one(tokens, cmd, data);
}
