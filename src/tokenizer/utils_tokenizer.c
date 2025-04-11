/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 22:18:58 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	handle_token_word(t_tokenizer_state *state, const char *str, t_data *data)
{
	int	status;

	while (str[state->j] && (!ft_isspace((unsigned char)str[state->j])
			|| state->inside_quotes))
	{
		status = process_quotes_and_redirects(state, str, data);
		if (status != 0)
		{
			if (status == MALLOC_ERROR || status == ERROR)
				return (status);
			continue ;
		}
		status = process_expansion(state, str, data);
		if (status != 0)
		{
			if (status == MALLOC_ERROR)
				return (MALLOC_ERROR);
			if (status == 2)
				break ;
			continue ;
		}
		if (add_to_buffer(state, str) == MALLOC_ERROR)
			return (MALLOC_ERROR);
	}
	return (create_word_token(state));
}
