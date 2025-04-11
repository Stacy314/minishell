/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 21:18:37 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*create_token(const char *value, t_token_type type, int index,
		bool touch_quotes)
{
	t_token	*new;

	new = ft_calloc(sizeof(t_token), 1);
	if (!new)
	{
		perror("calloc");
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		perror("strdup");
		free(new);
		return (NULL);
	}
	new->type = type;
	new->index = index;
	new->touch_quotes = touch_quotes;
	return (new);
}

int	create_word_token(t_tokenizer_state *state)
{
	int	j;

	if (state->k == 0 && !state->empty_quotes)
		return (0);
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++,
			false);
	if (!state->tokens[state->i])
	{
		j = 0;
		while (j < state->i)
		{
			if (state->tokens[j])
				(free(state->tokens[j]->value), free(state->tokens[j]));
			j++;
		}
		return (free(state->tokens), perror("create_token"), MALLOC_ERROR);
	}
	state->empty_quotes = 0;
	state->i++;
	state->k = 0;
	state->buffer[0] = '\0';
	return (1);
}

static int	tokenize_loop(const char *str, t_tokenizer_state *state,
	t_data *data)
{
	while (str[state->j])
	{
		skip_spaces(str, state);
		if (str[state->j] == '\0')
			break ;
		if (!state->inside_quotes && (is_pipe(str[state->j])
				|| is_redirect(str[state->j])))
		{
			if (handle_pipe_or_redirect(str, state, data) != SUCCESS)
				return (MALLOC_ERROR);
			continue ;
		}
		if (process_token_word(str, state, data) == MALLOC_ERROR)
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}

int	heredoc_count(const char *str, t_data *data)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (count > HEREDOC_MAX)
		{
			data->heredoc_count = count;
			return (0);
		}
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i += 2;
			count++;
		}
		i++;
	}
	return (1);
}

t_token	**split_to_tokens(const char *str, t_data *data)
{
	t_tokenizer_state	state;
	t_token				**tokens;

	if (!str)
		return (NULL);
	heredoc_count(str, data);
	tokens = ft_calloc(ft_strlen(str) + 1, sizeof(t_token *));
	if (!tokens)
		return (NULL);
	if (!init_state(&state, tokens))
		return (free(tokens), NULL);
	if (tokenize_loop(str, &state, data) == MALLOC_ERROR)
		return (NULL);
	if (state.inside_quotes)
	{
		write_error("minishell: syntax error: unclosed quotes\n");
		free(state.buffer);
		free_tokens(tokens);
		return (data->exit_status = 2, NULL);
	}
	tokens[state.i] = NULL;
	free(state.buffer);
	return (tokens);
}
