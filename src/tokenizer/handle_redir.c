/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 22:28:12 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	add_redirect_token(t_tokenizer_state *state, const char *symbol,
		t_token_type type, int advance)
{
	int j;

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

int	handle_redirection_tok(t_tokenizer_state *state, const char *str)
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

