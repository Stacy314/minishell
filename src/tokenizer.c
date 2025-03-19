/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/19 19:59:10 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdio.h> // Для printf

void debug_print_tokens(t_token **tokens)
{
    if (!tokens)
    {
        printf("No tokens (tokens == NULL)\n");
        return;
    }

    int i = 0;
    while (tokens[i])
    {
        printf("Token[%d]: Type = %d, Value = \"%s\", Index = %d\n",
            i,
            tokens[i]->type,
            tokens[i]->value,
            tokens[i]->index
        );
        i++;
    }
    printf("End of tokens (null)\n");
}


t_token	*create_token(const char *value, t_token_type type, int index)
{
	t_token	*new;

	new = ft_calloc(sizeof(t_token), 1);
	if (!new)
	{
		perror("calloc");
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		perror("strdup");
		free(new);
		return (NULL);
	}
	new->type = type;
	new->index = index;
	return (new);
}

int	handle_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	char	*expanded;
	size_t	len;

	if (str[state->j] == '$' && state->quote_type != '\'')
	{
		expanded = expand_variable(str, &state->j, data);
		if (!expanded || !*expanded)
		{
			data->exit_status = 1;
			return (free(expanded), -1);
		}
		len = ft_strlen(expanded);
		while (state->k + len >= (size_t)state->buffer_size)
		{
			if (expand_buffer(state) == -1)
				return (free(expanded), -1);
		}
		ft_strlcpy(&state->buffer[state->k], expanded, state->buffer_size - state->k);
		state->k += len;
		free(expanded);
		//state->j++;
		return (1);
	}
	return (0);
}

int handle_redirection(t_tokenizer_state *state, const char *str)
{
	if (str[state->j] == '>' && str[state->j + 1] == '>')
		return add_redirect_token(state, ">>", APPEND, 2);
	else if (str[state->j] == '<' && str[state->j + 1] == '<')
		return add_redirect_token(state, "<<", HEREDOC, 2);
	else if (str[state->j] == '>')
		return add_redirect_token(state, ">", REDIRECT_OUT, 1);
	else if (str[state->j] == '<')
		return add_redirect_token(state, "<", REDIRECT_IN, 1);
	return 0;
}

int handle_quotes_and_redirects(t_tokenizer_state *state, const char *str)
{
	if (is_quote(str[state->j]) &&
		(!state->inside_quotes || str[state->j] == state->quote_type))
		return update_quote_state(state, str[state->j]);
	if (!state->inside_quotes && is_redirect(str[state->j]))
	{
		if (state->k > 0 && flush_word_before_redirect(state) == -1)
			return -1;
		if (handle_redirection(state, str) == -1)
			return -1;
		return 1;
	}
	return 0;
}

t_token **split_to_tokens(const char *str, t_data *data)
{
	t_tokenizer_state state;
	t_token **tokens;

	if (!str || ft_strlen(str) == 0)
		return NULL;
	tokens = ft_calloc(ft_strlen(str) + 1, sizeof(t_token *));
	if (!tokens)
		return (perror("calloc"), NULL);
	initialize_state(&state, tokens);
	if (tokenize_loop(str, &state, tokens, data) == -1)
		return (NULL);
	printf("inside_quotes: %d\n", state.inside_quotes);
	if (state.inside_quotes)
	{
		write_error("minishell: syntax error: unclosed quotes\n");
		return (free_tokens(tokens), free(state.buffer), (t_token **)(-1));
	}
	tokens[state.i] = NULL;
	free(state.buffer);
	debug_print_tokens(tokens);
	return tokens;
}
