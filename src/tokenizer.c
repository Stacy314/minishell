/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/24 17:15:02 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	debug_print_tokens(t_token **tokens)
{
	int	i;

	if (!tokens)
	{
		printf("No tokens (tokens == NULL)\n");
		return ;
	}
	i = 0;
	while (tokens[i])
	{
		printf("Token[%d]: Type = %d, Value = \"%s\", Index = %d\n", i,
			tokens[i]->type, tokens[i]->value, tokens[i]->index);
		i++;
	}
	printf("End of tokens (null)\n");
}

int	handle_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	char	*expanded;
	size_t	len;

	if (str[state->j] == '$' && state->quote_type != '\'')
	{
		expanded = expand_variable(str, &state->j, data);
		// if (!expanded || !*expanded)
		//{
		//	data->exit_status = 1;
		//	return (free(expanded), -1);
		//}
		len = ft_strlen(expanded);
		while (state->k + len >= (size_t)state->buffer_size)
		{
			if (expand_buffer(state) == -1)
				return (free(expanded), -1);
		}
		ft_strlcpy(&state->buffer[state->k], expanded, state->buffer_size
			- state->k);
		state->k += len;
		free(expanded);
		// state->j++;
		return (1);
	}
	return (0);
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

int	add_redirect_token(t_tokenizer_state *state, const char *symbol,
		t_token_type type, int advance)
{
	state->tokens[state->i] = create_token(symbol, type, (state->index)++);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->j += advance;
	state->i++;
	return (0);
}

int	handle_redirection(t_tokenizer_state *state, const char *str)
{
	if (str[state->j] == '>' && str[state->j + 1] == '>')
	{
		if (add_redirect_token(state, ">>", APPEND, 2) == -1)
			return (/*free, */free_tokens(state->tokens), 1);
	}
	else if (str[state->j] == '<' && str[state->j + 1] == '<')
	{
		if (add_redirect_token(state, "<<", HEREDOC, 2) == -1)
			return (/*free, */ free_tokens(state->tokens), 1);
	}
	else if (str[state->j] == '>')
	{
		if (add_redirect_token(state, ">", REDIRECT_OUT, 1) == -1)
			return (/*free, */ free_tokens(state->tokens), 1);
	}
	else if (str[state->j] == '<')
	{
		if (add_redirect_token(state, "<", REDIRECT_IN, 1) == -1)
			return (/*free, */ free_tokens(state->tokens), 1);
	}
	return (0);
}

static int	tokenize_loop(const char *str, t_tokenizer_state *state, t_token **tokens,
		t_data *data)
{
	while (str[state->j])
	{
		skip_spaces(str, state);
		if (str[state->j] == '\0')
			break ;
		if (is_logical_operator(str, state))
			continue ;
		if (is_pipe_operator(str, state))
			continue ;
		if (!state->inside_quotes && is_redirect(str[state->j]))
		{
			flush_buffer_to_token(state);
			if (handle_redirection(state, str) == -1)
				return (cleanup_and_null(tokens, state), -1);
			continue ;
		}
		if (handle_token_word(state, str, data) == -1)
			return (cleanup_and_null(tokens, state), -1);
	}
	return (0);
}

t_token	**split_to_tokens(const char *str, t_data *data)
{
	t_tokenizer_state	state;
	t_token				**tokens;

	if (!str || ft_strlen(str) == 0)
		return (NULL);
	tokens = ft_calloc(ft_strlen(str) + 1, sizeof(t_token *));
	if (!tokens)
		return (perror("calloc"), NULL);
	initialize_state(&state, tokens);
	if (tokenize_loop(str, &state, tokens, data) == -1)
		return (free_tokens(tokens), free(tokens), NULL);
	if (state.inside_quotes)
	{
		write_error("minishell: syntax error: unclosed quotes\n");
		return (free_tokens(tokens), free(state.buffer), (t_token **)(-1));
	}
	tokens[state.i] = NULL;
	free(state.buffer);
	// debug_print_tokens(tokens);
	return (tokens);
}
