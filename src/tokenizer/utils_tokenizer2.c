/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 21:19:41 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 21:59:08 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_pipe_or_redirect(const char *str, t_tokenizer_state *state,
		t_data *data)
{
	int	redir_result;

	if (str[state->j] == '|')
	{
		if (create_pipe_operator(str, state) == MALLOC_ERROR)
			return (MALLOC_ERROR);
		state->j++;
	}
	else
	{
		redir_result = handle_quotes_and_redirects(state, str, data);
		if (redir_result == MALLOC_ERROR)
			return (MALLOC_ERROR);
		if (redir_result == 2)
			return (ERROR);
	}
	return (SUCCESS);
}

int	process_token_word(const char *str, t_tokenizer_state *state, t_data *data)
{
	if (handle_token_word(state, str, data) == MALLOC_ERROR)
		return (MALLOC_ERROR);
	return (SUCCESS);
}
