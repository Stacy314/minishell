/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 23:42:10 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_quote_word(t_tokenizer_state *state, const char *str, t_data *data)

{
	int result;

	if(!str[state->j])
		return (0);
	if (state->inside_quotes && str[state->j] == state->quote_type)
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return (-1);
		if (create_word_token(state, str) == -1)
			return (-1);
	}
	else if (str[state->j] == state->quote_type)
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return (-1);
		if (create_word_token(state, str) == -1)
			return (-1);
	}
	else
	{
		result = handle_expansion(state, str, data);
		if (result == -1)
			return (-1);
		if (result == 2)
			if (create_nothing_token(str, state) == -1)
				return (-1);
	}
	return (0);
}

int	handle_quotes_and_redirects(t_tokenizer_state *state, const char *str)
{
	if (is_quote(str[state->j]) && (!state->inside_quotes
			|| str[state->j] == state->quote_type))
		return (update_quote_state(state, str[state->j]));
	if (!state->inside_quotes && is_pipe(str[state->j]))
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return (-1);
		if (create_pipe_token(state) == -1)
			return (-1);
	}
	else if (!state->inside_quotes && is_redirect(str[state->j]))
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return (-1);
		if (handle_redirection_tok(state, str) == -1)
			return (-1);
		return (1);
	}
	return (0);
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
		if (state->k == 0)           // marat
			state->empty_quotes = 1; // marat
	}
	state->j++;
	return (1);
}

int	quote_checker(const char *str, t_tokenizer_state *state)
{
	if (str[state->j] == '\'' || str[state->j] == '\"')
	{
		if (state->inside_quotes)
			state->inside_quotes = 0;
		else
			state->inside_quotes = 1;
		state->quote_type = str[state->j];
		state->j++;
	}
	if ((str[state->j] == '\'' || str[state->j] == '\"')
			&& state->inside_quotes)
	{
		if (create_nothing_token(str, state) == -1)
			return (-1);
		state->inside_quotes = 0;
	}
	return (0);
}
