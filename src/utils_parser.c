/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:00:25 by mgallyam          #+#    #+#             */
/*   Updated: 2025/03/19 16:28:09 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redirect_token(t_token *token)
{
	return (token->type == REDIRECT_IN || token->type == REDIRECT_OUT ||
		token->type == APPEND || token->type == HEREDOC);
}

int handle_redirect(t_cmd *cmd, t_token **tokens, t_data *data, int *i)
{
	if (!tokens[*i + 1] || tokens[*i + 1]->type != WORD)
	{
		write_error("minishell: syntax error near unexpected token `%s'\n",
			tokens[*i + 1] ? tokens[*i + 1]->value : "newline");
		data->exit_status = 2;
		return 0;
	}
	if (!parse_redirects(cmd, tokens[*i + 1], tokens[*i]->type))
		return 0;
	(*i)++;
	return 1;
}

int fill_cmd(t_cmd *cmd, t_token **tokens, t_data *data, int *i)
{
	while (tokens[*i] && tokens[*i]->type != PIPE)
	{
		if (is_redirect_token(tokens[*i]))
		{
			if (!handle_redirect(cmd, tokens, data, i))
				return 0;
		}
		else
		{
			cmd->args = append_to_args(cmd->args, tokens[*i]->value);
			if (!cmd->args)
				return 0;
		}
		(*i)++;
	}
	return 1;
}

int build_command_list(t_cmd **head, t_token **tokens, t_data *data, int *i)
{
	t_cmd *prev = NULL;
	t_cmd *current = NULL;
	while (tokens[*i])
	{
		current = init_new_cmd();
		if (!current)
			return 0;
		if (!*head)
			*head = current;
		else
			prev->next = current;
		if (!fill_cmd(current, tokens, data, i))
			return 0;
		prev = current;
		if (tokens[*i] && tokens[*i]->type == PIPE)
			(*i)++;
	}
	return 1;
}
