/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/17 21:52:41 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//"" (: command not found)  &   //echo '' ""
// this should be fixed in execution part:
// if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
// {
//     ft_putstr_fd("minishell: '", STDERR_FILENO);
//     ft_putstr_fd(cmd->args[0], STDERR_FILENO); // даже если пусто
//     ft_putendl_fd("': command not found", STDERR_FILENO);
//     return (127);
// }

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
		builtin_env(data, cmd);
	else if (ft_strncmp(cmd->args[0], "exit", ft_strlen(cmd->args[0])) == 0)
		builtin_exit(cmd, data);
	else
	{
		data->exit_status = execute_command(cmd->args[0], data, cmd->args, env);
	}
}

void	execute(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	if (contains_special_char(tokens, PIPE))
	{
		execute_pipeline(tokens, cmd, data, env);
		return ;
	}
	if (cmd->heredoc_delimiter || (cmd->input_redirects
			&& cmd->input_redirects[0]) || (cmd->output_redirects
			&& cmd->output_redirects[0]) || (cmd->append_redirects
			&& cmd->append_redirects[0])) // cmd->input_redirects[0]
	{
		if (execute_redirection(cmd, data, env) == 1)
			return ;
	}
	else
		execute_for_one(tokens, cmd, data, env);
	//if (cmd)
	//	free_cmd(cmd);
	if (tokens)
		free_tokens(tokens);
}
