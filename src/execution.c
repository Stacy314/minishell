/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 20:06:26 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	(void)tokens;
	if (!cmd->args || !cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "echo", ft_strlen(cmd->args[0])) == 0)
		builtin_echo(cmd, data);
	else if (ft_strncmp(cmd->args[0], "cd", ft_strlen(cmd->args[0])) == 0)
		builtin_cd(cmd, data);
	else if (ft_strncmp(cmd->args[0], "pwd", ft_strlen(cmd->args[0])) == 0)
		builtin_pwd(cmd, data);
	else if (ft_strncmp(cmd->args[0], "export", ft_strlen(cmd->args[0])) == 0)
		builtin_export(cmd, data);
	else if (ft_strncmp(cmd->args[0], "unset", ft_strlen(cmd->args[0])) == 0)
		builtin_unset(cmd, data);
	else if (ft_strncmp(cmd->args[0], "env", ft_strlen(cmd->args[0])) == 0)
		builtin_env(data);
	else if (ft_strncmp(cmd->args[0], "exit", ft_strlen(cmd->args[0])) == 0)
		builtin_exit(cmd, data);
	else
	{
		data->exit_status = execute_command(cmd->args[0], data, cmd->args, env);
	}
}

void	execute(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	if (cmd->heredoc_delimiter ||
		(cmd->input_redirects && cmd->input_redirects[0]) ||
		(cmd->output_redirects && cmd->output_redirects[0]) ||
		(cmd->append_redirects && cmd->append_redirects[0]))
	{
		if (execute_redirection(cmd, data, env) == 1)
			return;
	}

	if (contains_special_char(tokens, PIPE))
	{
		execute_pipeline(tokens, cmd, data, env);
	}
	else
		execute_for_one(tokens, cmd, data, env);

	if (cmd)
		free_cmd(cmd);
	if (tokens)
		free_tokens(tokens);
}
