/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/08 22:19:02 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
int	handle_token_word(t_tokenizer_state *state, const char *str, t_data *data)
{
	int	result;

	while (str[state->j] && (!ft_isspace((unsigned char)str[state->j])
			|| state->inside_quotes) /*&& str[state->j] != '|'
		&& !is_redirect(str[state->j])*/)
	{
		result = handle_quotes_and_redirects(state, str, data);
		if (result == MALLOC_ERROR)
			return (MALLOC_ERROR);
		if (result == 1)
			continue ;
		if (result == 2)
			return (ERROR);
		result = handle_expansion(state, str, data);
		if (result == MALLOC_ERROR)
			return (MALLOC_ERROR);
		if (result == 1 || result == 2)
			continue ;
		if (result == 3)
		{
			if (create_nothing_token(str, state) == MALLOC_ERROR)
				return (MALLOC_ERROR);
			continue ;
		}
		if (result == 2)
			break ;
		if (state->k >= state->buffer_size - 1)
			if (expand_buffer(state) == MALLOC_ERROR)
				return (MALLOC_ERROR);
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
		state->empty_quotes = 0;
	}
	else if (state->inside_quotes && c == state->quote_type)
	{
		state->inside_quotes = 0;
		state->quote_type = 0;
		if (state->k == 0)
			state->empty_quotes = 1;
	}
	state->j++;
	return (1);
}

int	flush_word_before_redirect(t_tokenizer_state *state)
{
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++,
			false);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->i++;
	state->k = 0;
	return (0);
}
