/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:44:45 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 21:15:33 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	fill_heredoc_buffer(t_tokenizer_state *state,
	const char *str, bool *touch_quotes)
{
	int		k;
	char	quote;

	k = 0;
	while (str[state->j] && !ft_isspace(str[state->j])
		&& str[state->j] != '<' && str[state->j] != '>' && str[state->j] != '|')
	{
		if (str[state->j] == '\'' || str[state->j] == '\"')
		{
			*touch_quotes = true;
			quote = str[state->j++];
			while (str[state->j] && str[state->j] != quote)
				state->buffer[k++] = str[state->j++];
			if (str[state->j] == quote)
				state->j++;
		}
		else
			state->buffer[k++] = str[state->j++];
	}
	state->buffer[k] = '\0';
	return (0);
}

int	heredoc_delimiter_valid(t_tokenizer_state *state, const char *str)
{
	skip_spaces(str, state);
	if (str[state->j] == '\0')
		return (1);
	if (str[state->j] == '|' || is_redirect(str[state->j]))
		return (1);
	return (0);
}

int	check_quotes_and_redirects(t_tokenizer_state *state, const char *str,
	t_data *data)
{
	int	result;

	result = handle_quotes_and_redirects(state, str, data);
	if (result == MALLOC_ERROR || result == 2)
		return (result);
	if (result == 1)
		return (1);
	return (0);
}

int	check_expansion_result(int result, t_tokenizer_state *state,
	const char *str)
{
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
