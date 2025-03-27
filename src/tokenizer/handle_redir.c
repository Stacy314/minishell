/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/25 15:12:32 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_redirection(t_tokenizer_state *state, const char *str)
{
	if (str[state->j] == '>' && str[state->j + 1] == '>')
		return (add_redirect_token(state, ">>", APPEND, 2));
	else if (str[state->j] == '<' && str[state->j + 1] == '<')
		return (add_redirect_token(state, "<<", HEREDOC, 2));
	else if (str[state->j] == '>')
		return (add_redirect_token(state, ">", REDIRECT_OUT, 1));
	else if (str[state->j] == '<')
		return (add_redirect_token(state, "<", REDIRECT_IN, 1));
	return (0);
}

int	handle_quotes_and_redirects(t_tokenizer_state *state, const char *str)
{
	if (is_quote(str[state->j]) && (!state->inside_quotes
			|| str[state->j] == state->quote_type))
		return (update_quote_state(state, str[state->j]));
	if (!state->inside_quotes && is_redirect(str[state->j]))
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return (-1);
		if (handle_redirection(state, str) == -1)
			return (-1);
		return (1);
	}
	return (0);
}
