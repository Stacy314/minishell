/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/27 13:47:03 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_for_one(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	int	i;

	i = 0;
	{
		if (tokens[i]->type == WORD)
		{
			if (ft_strncmp(tokens[i]->value, "echo", 4) == 0)
				return ((void)builtin_echo(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "cd", 2) == 0)
				return ((void)builtin_cd(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "pwd", 3) == 0)
				return ((void)builtin_pwd(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "export", 6) == 0)
				return ((void)builtin_export(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "unset", 5) == 0)
				return ((void)builtin_unset(cmd, data));
			else if (ft_strncmp(tokens[i]->value, "env", 3) == 0)
				return ((void)builtin_env(data));
			else if (ft_strncmp(tokens[i]->value, "exit", 4) == 0)
				return ((void)builtin_exit(cmd, data));
			else
				data->exit_status = execute_command(cmd->args[0], data,
						cmd->args, env);
		}
		i++;
	}
}

void	execute(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	if (cmd->heredoc_delimiter || cmd->input_redirect || cmd->output_redirect
		|| cmd->append_redirect)
	{
		if (execute_redirection(cmd, env) == 1)
			return ;
	}
	if (contains_special_char(tokens, '|'))
	{
		printf("im here1\n");
		execute_pipeline(&cmd, data, env);
	}
	else
		execute_for_one(tokens, cmd, data, env);
	if (cmd)
		free_cmd(cmd);
	if (tokens)
		free_tokens(tokens);
}
