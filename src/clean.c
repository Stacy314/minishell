/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 19:01:53 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
void	free_bool_array(bool *arr)
{
	if (arr)
		free(arr);
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next;
		free_array(cmd->args);
		free_array(cmd->input_redirects);
		free_array(cmd->output_redirects);
		free_array(cmd->append_redirects);
		free_array(cmd->heredoc_delimiter);
		free(cmd->pipe_pids);
		free(cmd->heredoc_touch_quotes);

		if (cmd->heredoc_delimiter && cmd->heredoc_fd > 0 /*&& cmd->next*/)
			close(cmd->heredoc_fd);
		free(cmd);
		cmd = tmp;
	}
}

void	free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (!tokens && !tokens[i])
		return ;
	while (tokens[i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

void	*cleanup_and_null(t_tokenizer_state *state)
{
	if (!state)
		return (NULL);
	if (state->buffer)
		free(state->buffer);
	if (state->tokens)
		free_tokens(state->tokens);
	free(state);
	return (NULL);
}

void free_all(t_data *data, t_token **tokens, t_cmd *cmd)
{
	if (data->input)
		free(data->input);
	if (data->env)
	 	free_array(data->env);
	if (tokens)
		free_tokens(tokens);
	if (cmd)
		free_cmd(cmd);
}

void free_main(t_data *data, t_token **tokens, t_cmd *cmd)
{
	if (data->input)
		free(data->input);
	if (tokens)
		free_tokens(tokens);
	if (cmd)
		free_cmd(cmd);
}