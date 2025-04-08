/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 22:31:21 by apechkov         ###   ########.fr       */
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
		return (MALLOC_ERROR);
	}
	state->j += advance;
	state->i++;
	return (SUCCESS);
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
		return (SUCCESS);
	if (str[state->j] == '|' || is_redirect(str[state->j]))
		return (SUCCESS);
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
		return (perror("heredoc delimiter token alloc"), MALLOC_ERROR);
	state->i++;
	state->k = 0;
	return (SUCCESS);
}
int	ambiguous_check(t_tokenizer_state *state, const char *str, t_data *data)
{
	int		start;
	int		end;
	char	*var_name;
	char	*value;

	(void)data;
	skip_spaces(str, state);
	if (str[state->j] == '\0')
		return (SUCCESS);
	start = state->j + 1;
	end = start;
	if (str[state->j] != '$' || !str[start])
		return (SUCCESS);
	if (str[start] == '?')
		return (SUCCESS);
	while (str[end] && !ft_isspace(str[end]) && str[end] != '<'
		&& str[end] != '>' && str[end] != '|')
		end++;
	var_name = ft_substr(str, start, end - start);
	if (!var_name)
		return (MALLOC_ERROR);
	value = getenv(var_name);
	if (!value || value[0] == '\0')
		return (write_error("minishell: $%s: ambiguous redirect\n", var_name),
			free(var_name), ERROR);
	free(var_name);
	return (SUCCESS);
}

int	handle_redirection_tok(t_tokenizer_state *state, const char *str,
		t_data *data)
{
	if (str[state->j] == '>' && str[state->j + 1] == '>')
	{
		add_redirect_token(state, ">>", APPEND, 2);
		return (ambiguous_check(state, str, data));
	}
	else if (str[state->j] == '<' && str[state->j + 1] == '<')
	{
		add_redirect_token(state, "<<", HEREDOC, 2);
		return (handle_heredoc_delimiter(state, str));
	}
	else if (str[state->j] == '>')
	{
		add_redirect_token(state, ">", REDIRECT_OUT, 1);
		return (ambiguous_check(state, str, data));
	}
	else if (str[state->j] == '<')
		return (add_redirect_token(state, "<", REDIRECT_IN, 1));
	return (SUCCESS);
}

int	handle_quotes_and_redirects(t_tokenizer_state *state, const char *str,
		t_data *data)
{
	int	redir_result;

	if (is_quote(str[state->j]) && (!state->inside_quotes
			|| str[state->j] == state->quote_type))
		return (update_quote_state(state, str[state->j]));
	if (!state->inside_quotes && is_redirect(str[state->j]))
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return (-1);
		redir_result = handle_redirection_tok(state, str, data);
		if (redir_result == MALLOC_ERROR)
			return (MALLOC_ERROR);
		if (redir_result == ERROR)
			return (2);
		return (SUCCESS);
	}
	return (0);
}
