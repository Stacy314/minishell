/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handle_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:45:00 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 22:39:15 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_quotes_and_redirects(t_tokenizer_state *state, const char *str,
		t_data *data)
{
	int	result;

	result = handle_quotes_and_redirects(state, str, data);
	if (result == MALLOC_ERROR)
		return (MALLOC_ERROR);
	if (result == 1)
		return (1);
	if (result == 2)
		return (ERROR);
	return (0);
}

int	process_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	int	result;

	result = handle_expansion(state, str, data);
	if (result == MALLOC_ERROR)
		return (MALLOC_ERROR);
	if (result == 1 || result == 2)
		return (1);
	if (result == 3)
	{
		if (create_nothing_token(str, state) == MALLOC_ERROR)
			return (MALLOC_ERROR);
		return (1);
	}
	if (result == 2)
		return (2);
	return (0);
}

int	add_to_buffer(t_tokenizer_state *state, const char *str)
{
	if (state->k >= state->buffer_size - 1)
		if (expand_buffer(state) == MALLOC_ERROR)
			return (MALLOC_ERROR);
	state->buffer[state->k++] = str[state->j++];
	return (0);
}
