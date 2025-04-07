/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 15:35:14 by apechkov         ###   ########.fr       */
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
int	create_nothing_token(const char *str, t_tokenizer_state *state)
{
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, NOTHING,
			state->index++, false);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->i++;
	state->k = 0;
	skip_spaces(str, state);
	return (0);
}
 int	handle_token_word(t_tokenizer_state *state, const char *str,
		t_data *data)
{
	int	result;

	while (str[state->j] && (!ft_isspace((unsigned char)str[state->j])
			|| state->inside_quotes))
	{
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
		if (result == 2)
		{
			if (create_nothing_token(str, state) == -1)
				return (-1);
			continue ;
		}

		if (state->k >= state->buffer_size - 1)
			if (expand_buffer(state) == -1)
				return (-1);

		state->buffer[state->k++] = str[state->j++];
	}
	return (create_word_token(state));
}

int	update_quote_state(t_tokenizer_state *state, char c)
{
	if (!state->inside_quotes)
	{
		state->inside_quotes = 1;
		state->quote_type = c;
		state->empty_quotes = 0; // marat
	}
	else if (state->inside_quotes && c == state->quote_type)
	{
		state->inside_quotes = 0;
		state->quote_type = 0;
		if (state->k == 0) // marat
			state->empty_quotes = 1; //marat
	}
	state->j++;
	return (1);
}

int	flush_word_before_redirect(t_tokenizer_state *state)
{
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++, false);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->i++;
	state->k = 0;
	return (0);
}
