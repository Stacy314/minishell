/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 16:58:46 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
//{
//	int	i;

//	i = 0;
//	{
//		if (tokens[i]->type == WORD)
//		{
//			if (ft_strncmp(tokens[i]->value, "echo",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_echo(cmd, data));
//			else if (ft_strncmp(tokens[i]->value, "cd",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_cd(cmd, data));
//			else if (ft_strncmp(tokens[i]->value, "pwd",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_pwd(cmd, data));
//			else if (ft_strncmp(tokens[i]->value, "export",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_export(cmd, data));
//			else if (ft_strncmp(tokens[i]->value, "unset",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_unset(cmd, data));
//			else if (ft_strncmp(tokens[i]->value, "env",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_env(data));
//			else if (ft_strncmp(tokens[i]->value, "exit",
					//ft_strlen(tokens[i]->value)) == 0)
//				return ((void)builtin_exit(cmd, data));
//			else
//				data->exit_status = execute_command(cmd->args[0], data,
//						cmd->args, env);
//		}
//		i++;
//	}
//}

void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	(void)tokens;
	if (!cmd->args || !cmd->args[0])
		return ;
	if (strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd, data);
	else if (strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd, data);
	else if (strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd, data);
	else if (strcmp(cmd->args[0], "export") == 0)
		builtin_export(cmd, data);
	else if (strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(cmd, data);
	else if (strcmp(cmd->args[0], "env") == 0)
		builtin_env(data);
	else if (strcmp(cmd->args[0], "exit") == 0)
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
		(cmd->append_redirects && cmd->append_redirects[0]))  // Лучшая проверка
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
