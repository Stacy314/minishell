/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:47:44 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/08 13:48:55 by mgallyam         ###   ########.fr       */
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

int	checking_token_word(t_tokenizer_state *state, t_data *data, const char *str)
{
	int	result;

	result = handle_quotes_and_redirects(state, str);
	if (result == -1 || result == 1)
		return (result);
	result = handle_expansion(state, str, data);
	if (result == -1 || result == 1)
		return (result);
	if (result == 2)
	{
		if (create_nothing_token(str, state) == -1)
			return (1);
	}
	return (0);
}
