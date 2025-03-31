/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/31 22:43:14 by anastasiia       ###   ########.fr       */
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
		printf("Token[%d]: Type = %d, Value = \"%s\", Index = %d\n", i,
			tokens[i]->type, tokens[i]->value, tokens[i]->index);
		i++;
	}
	printf("End of tokens (null)\n");
}

////////////////////////////////////////////////////////////////////////////

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
		//if (!expanded || !*expanded)
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
		return (1);
	}
	return (0);
}

int	create_word_token(t_tokenizer_state *state)
{
	if (state->k == 0 || state->buffer[0] == '\0')
		return (0);
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++);
	if (!state->tokens[state->i])
	{
		while (state->tokens[state->i] > 0)
		{
			free(state->tokens[state->i]);
			state->i--;
		}
		perror("create_token");
		return (-1);
	}
	state->i++;
	state->k = 0;
	state->buffer[0] = '\0';
	return (1);
}

static int	tokenize_loop(const char *str, t_tokenizer_state *state, /*t_token **tokens,*/
		t_data *data)
{
	while (str[state->j])
	{
		skip_spaces(str, state);
		if (str[state->j] == '\0')
			break ;
		if (is_pipe_operator(str, state)) // mem leak
			continue ;
		if (!state->inside_quotes && is_redirect(str[state->j]))
		{
			if (create_word_token(state) == -1)           // mem
				return (cleanup_and_null(state), -1);
			if (handle_redirection(state, str) == -1)         // mem  leak
				return (cleanup_and_null(state), -1);
			continue ;
		}
		if (handle_token_word(state, str, data) == -1) // mem
			return (-1);
	}
	return (0);
}

t_token	**split_to_tokens(const char *str, t_data *data)
{
	t_tokenizer_state	state;
	t_token				**tokens;

	if (!str)
		return (NULL);
	tokens = ft_calloc(ft_strlen(str) + 1, sizeof(t_token *)); // mem
	if (!tokens)
		return (perror("calloc"), NULL);
	if (!init_state(&state, tokens)) // mem
		return (free(tokens), NULL);
	if (tokenize_loop(str, &state,/* tokens,*/ data) == -1) // mem
		return (/* cleanup_and_null(&state),  */NULL);
	if (state.inside_quotes)
	{
		write_error("minishell: syntax error: unclosed quotes\n");  //exit code? 
		return (/* cleanup_and_null(&state),  */data->exit_status = 2, NULL);
	}
	tokens[state.i] = NULL;
	free(state.buffer);
	//debug_print_tokens(tokens);
	return (tokens);
}
