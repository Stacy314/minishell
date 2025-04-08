/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/08 16:28:10 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

////////////////////////////////////////////////////////////////////////////
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
		printf("Token[%d]: Type = %d, Value = \"%s\", Index = %d, Quote = %d\n",
			i, tokens[i]->type, tokens[i]->value, tokens[i]->index,
			tokens[i]->touch_quotes);
		i++;
	}
	printf("End of tokens (null)\n");
}

////////////////////////////////////////////////////////////////////////////

t_token	*create_token(const char *value, t_token_type type, int index,
		bool touch_quotes)
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
	new->touch_quotes = touch_quotes;
	return (new);
}

int	create_word_token(t_tokenizer_state *state)
{
	int	j;

	if (state->k == 0 && !state->empty_quotes)
		return (0);
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++,
			false);
	if (!state->tokens[state->i])
	{
		j = 0;
		while (j < state->i)
		{
			if (state->tokens[j])
			{
				free(state->tokens[j]->value);
				free(state->tokens[j]);
			}
			j++;
		}
		free(state->tokens);
		perror("create_token");
		return (MALLOC_ERROR);
	}
	state->empty_quotes = 0;
	state->i++;
	state->k = 0;
	state->buffer[0] = '\0';
	return (1);
}

static int	tokenize_loop(const char *str, t_tokenizer_state *state,
		t_data *data)
{
	int	redir_result;

	while (str[state->j])
	{
		skip_spaces(str, state);
		if (str[state->j] == '\0')
			break ;
		if (!state->inside_quotes && (is_pipe(str[state->j])
				|| is_redirect(str[state->j])))
		{
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
			continue ;
		}
		if (handle_token_word(state, str, data) == MALLOC_ERROR)
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}

int	heredoc_count(const char *str, t_data *data)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (count > HEREDOC_MAX)
		{
			data->heredoc_count = count;
			return (0);
		}
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i += 2;
			count++;
		}
		i++;
	}
	return (1);
}

t_token	**split_to_tokens(const char *str, t_data *data)
{
	t_tokenizer_state	state;
	t_token				**tokens;

	if (!str)
		return (NULL);
	heredoc_count(str, data);
	tokens = ft_calloc(ft_strlen(str) + 1, sizeof(t_token *));
	if (!tokens)
		return (perror("calloc"), NULL);
	if (!init_state(&state, tokens))
		return (free(tokens), NULL);
	if (tokenize_loop(str, &state, data) == MALLOC_ERROR)
		return (NULL);
	if (state.inside_quotes)
	{
		write_error("minishell: syntax error: unclosed quotes\n");
		return (data->exit_status = 2, NULL);
	}
	tokens[state.i] = NULL;
	free(state.buffer);
	//debug_print_tokens(tokens);
	return (tokens);
}
