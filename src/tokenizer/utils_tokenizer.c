/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/25 16:00:19 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	int	j;

	state->tokens[state->i] = create_token(symbol, type, (state->index)++);
	if (!state->tokens[state->i])
	{
		j = 0;
		while (j >= 0)
		{
			if (state->tokens[j])
			{
				free(state->tokens[j]->value);
				free(state->tokens[j]);
			}
			j--;
		}
		perror("failed create token");
		return (-1);
	}
	state->j += advance;
	state->i++;
	return (0);
}
