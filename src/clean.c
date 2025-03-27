/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/27 20:44:03 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//void	free_env(char **env)
//{
//	int	i;

//	i = 0;
//	if (!env)
//		return ;
//	while (env[i])
//	{
//		free(env[i]);
//		i++;
//	}
//	free(env);
//}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->input_redirects)
	{
		i = 0;
		while (cmd->input_redirects[i])
			free(cmd->input_redirects[i++]);
		free(cmd->input_redirects);
	}
	if (cmd->output_redirects)
	{
		i = 0;
		while (cmd->output_redirects[i])
			free(cmd->output_redirects[i++]);
		free(cmd->output_redirects);
	}
	if (cmd->append_redirects)
	{
		i = 0;
		while (cmd->append_redirects[i])
			free(cmd->append_redirects[i++]);
		free(cmd->append_redirects);
	}
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	free(cmd);
}

void	free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	*cleanup_and_null(t_token **tokens, t_tokenizer_state *state)
{
	free_tokens(tokens);
	free(state->buffer);
	return (NULL);
}
