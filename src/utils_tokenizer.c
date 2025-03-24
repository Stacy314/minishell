/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/24 14:26:53 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_redirect(char c)
{
	return (c == '>' || c == '<');
}

int	expand_buffer(t_tokenizer_state *state)
{
	char	*new_buffer;

	state->buffer_size *= 2;
	new_buffer = ft_calloc(state->buffer_size, sizeof(char));
	if (!new_buffer)
		return (-1);
	ft_strlcpy(new_buffer, state->buffer, state->buffer_size);
	free(state->buffer);
	state->buffer = new_buffer;
	return (0);
}

int	flush_buffer_to_token(t_tokenizer_state *state)
{
	if (state->k == 0 || state->buffer[0] == '\0')
		return (0);
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++);
	if (!state->tokens[state->i])
	{
		perror("create_token");
		return (-1);
	}
	state->i++;
	state->k = 0;
	state->buffer[0] = '\0';
	return (1);
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

void	skip_spaces(const char *str, t_tokenizer_state *state)
{
	while (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t'))
		state->j++;
}

int	is_logical_operator(const char *str, t_tokenizer_state *state)
{
	if (str[state->j] == '&' && str[state->j + 1] == '&')
	{
		flush_buffer_to_token(state);
		state->tokens[state->i++] = create_token("&&", LOGICAL_AND,
				state->index++);
		state->j += 2;
		return (1);
	}
	if (str[state->j] == '|' && str[state->j + 1] == '|')
	{
		flush_buffer_to_token(state);
		state->tokens[state->i++] = create_token("||", LOGICAL_OR,
				state->index++);
		state->j += 2;
		return (1);
	}
	return (0);
}

int	is_pipe_operator(const char *str, t_tokenizer_state *state)
{
	if (str[state->j] == '|')
	{
		flush_buffer_to_token(state);
		state->tokens[state->i++] = create_token("|", PIPE, state->index++);
		state->j++;
		return (1);
	}
	return (0);
}

int	handle_token_word(t_tokenizer_state *state, const char *str, t_data *data)
{
	int	result;

	while (str[state->j] && (!ft_isspace((unsigned char)str[state->j])
			|| state->inside_quotes))
	{
		if (state->inside_quotes && str[state->j] == '|')
		{
			state->buffer[state->k++] = str[state->j++];
			continue ;
		}
		result = handle_quotes_and_redirects(state, str);
		if (result == -1)
			return (-1);
		if (result == 1)
			continue ;
		result = handle_expansion(state, str, data);
		if (result == -1)
			return (-1);
		if (result == 1)
			continue ;
		if (state->k >= state->buffer_size - 1 && expand_buffer(state) == -1)
			return (-1);
		state->buffer[state->k++] = str[state->j++];
	}
	return (flush_buffer_to_token(state));
}

void	*cleanup_and_null(t_token **tokens, t_tokenizer_state *state)
{
	free_tokens(tokens);
	free(state->buffer);
	return (NULL);
}

int	tokenize_loop(const char *str, t_tokenizer_state *state, t_token **tokens,
		t_data *data)
{
	while (str[state->j])
	{
		skip_spaces(str, state);
		if (str[state->j] == '\0')
			break ;
		if (is_logical_operator(str, state))
			continue ;
		if (is_pipe_operator(str, state))
			continue ;
		if (!state->inside_quotes && is_redirect(str[state->j]))
		{
			flush_buffer_to_token(state);
			if (handle_redirection(state, str) == -1)
				return (cleanup_and_null(tokens, state), -1);
			continue ;
		}
		if (handle_token_word(state, str, data) == -1)
			return (cleanup_and_null(tokens, state), -1);
	}
	return (0);
}

int	update_quote_state(t_tokenizer_state *state, char c)
{
	if (!state->inside_quotes)
	{
		state->inside_quotes = 1;
		state->quote_type = c;
	}
	else if (state->inside_quotes && c == state->quote_type)
	{
		state->inside_quotes = 0;
		state->quote_type = 0;
	}
	state->j++;
	return (1);
}

int	flush_word_before_redirect(t_tokenizer_state *state)
{
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->i++;
	state->k = 0;
	return (0);
}

int	add_redirect_token(t_tokenizer_state *state, const char *symbol,
		t_token_type type, int advance)
{
	state->tokens[state->i] = create_token(symbol, type, (state->index)++);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->j += advance;
	state->i++;
	return (0);
}
