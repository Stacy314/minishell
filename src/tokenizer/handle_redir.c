/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 21:16:19 by mgallyam         ###   ########.fr       */
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

int	handle_heredoc_delimiter(t_tokenizer_state *state, const char *str)
{
	bool	touch_quotes;

	touch_quotes = false;
	if (heredoc_delimiter_valid(state, str))
		return (SUCCESS);
	if (fill_heredoc_buffer(state, str, &touch_quotes) == -1)
		return (MALLOC_ERROR);
	state->tokens[state->i] = create_token(state->buffer, WORD,
			state->index++, touch_quotes);
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
			free(var_name), data->exit_status = 1, ERROR);
	return (free(var_name), SUCCESS);
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
		return (SUCCESS);
	}
	return (0);
}
