/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 17:01:37 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	add_redirect_token(t_tokenizer_state *state, const char *symbol,
		t_token_type type, int advance)
{
	int	j;

	state->tokens[state->i] = create_token(symbol, type, (state->index)++,
			false);
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

static int	handle_heredoc_delimiter(t_tokenizer_state *state, const char *str)
{
	bool	touch_quotes;
	int		start;
	int		k;
	char	quote;

	touch_quotes = false;
	start = state->j;
	k = 0;
	skip_spaces(str, state);
	if (str[state->j] == '\0')
		return (write_error("minishell: syntax error near unexpected token `newline'\n"),
			-1);
	if (str[state->j] == '|' || is_redirect(str[state->j]))
		return (write_error("minishell: syntax error near unexpected token `%c'\n",
				str[state->j]), -1);
	while (str[state->j] && !ft_isspace(str[state->j]) && str[state->j] != '<'
		&& str[state->j] != '>' && str[state->j] != '|')
	{
		if (str[state->j] == '\'' || str[state->j] == '\"')
		{
			touch_quotes = true;
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
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++,
			touch_quotes);
	if (!state->tokens[state->i])
		return (perror("heredoc delimiter token alloc"), -1);
	state->i++;
	state->k = 0;
	return (0);
}

int	handle_redirection_tok(t_tokenizer_state *state, const char *str)
{
	if (str[state->j] == '>' && str[state->j + 1] == '>')
		return (add_redirect_token(state, ">>", APPEND, 2));
	else if (str[state->j] == '<' && str[state->j + 1] == '<')
	{
		add_redirect_token(state, "<<", HEREDOC, 2);
		return (handle_heredoc_delimiter(state, str));
	}
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
		if (handle_redirection_tok(state, str) == -1)
			return (-1);
		return (1);
	}
	return (0);
}
