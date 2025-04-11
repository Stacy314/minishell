/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 12:55:10 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_initial_syntax_errors(t_token **tokens, t_data *data)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == PIPE)
		{
			if (i == 0 || !tokens[i + 1] || tokens[i + 1]->type == PIPE)
			{
				write_error("minishell: syntax error near unexpected token `|'\n");
				data->exit_status = 2;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

char	**append_to_args(char **args, char *new_arg)
{
	int		len;
	char	**new_args;
	int		i;

	len = 0;
	if (args)
	{
		while (args[len])
			len++;
	}
	new_args = ft_calloc(sizeof(char *) * (len + 2), 1);
	if (!new_args)
		return (perror("calloc"), NULL);
	i = 0;
	while (i < len)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[len] = ft_strdup(new_arg);
	new_args[len + 1] = NULL;
	free(args);
	return (new_args);
}

int	fill_cmd(t_cmd *cmd, t_token **tokens, t_data *data, int *i)
{
	while (tokens[*i] && tokens[*i]->type != PIPE)
	{
		if (tokens[*i]->type == NOTHING && tokens[*i + 1] && tokens[*i + 1]->type != PIPE)
		{
			(*i)++;
			continue ;
		}
		if (tokens[*i]->type == REDIRECT_IN || tokens[*i]->type == REDIRECT_OUT
			|| tokens[*i]->type == APPEND || tokens[*i]->type == HEREDOC)
		{
			if (!handle_redirection_parser(cmd, tokens, data, i))
				return (0);
		}
		else
		{
			cmd->args = append_to_args(cmd->args, tokens[*i]->value);
			if (!cmd->args)
				return (0);
		}
		(*i)++;
	}
	return (1);
}

int	build_command_list(t_cmd **head, t_token **tokens, t_data *data, int *i)
{
	t_cmd	*prev;
	t_cmd	*current;

	prev = NULL;
	current = NULL;
	while (tokens[*i])
	{
		current = init_new_cmd();
		if (!current)
			return (0);
		if (!*head)
			*head = current;
		else
			prev->next = current;
		if (!fill_cmd(current, tokens, data, i))
			return (0);
		prev = current;
		if (tokens[*i] && tokens[*i]->type == PIPE)
			(*i)++;
	}
	return (1);
}

t_cmd	*parse_tokens(t_token **tokens, t_data *data)
{
	t_cmd	*head;
	int		i;

	head = NULL;
	i = 0;
	if (check_initial_syntax_errors(tokens, data))
		return (NULL);
	if (!build_command_list(&head, tokens, data, &i))
	{
		free_cmd(head);
		return (NULL);
	}
	return (head);
}
