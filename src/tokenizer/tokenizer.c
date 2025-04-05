/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 23:45:29 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/chardefs.h>

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

static int	tokenize_loop(const char *str, t_tokenizer_state *state,
		t_data *data)
{
	if(!str[state->j])
		return (0);
	while (str[state->j])
	{
		if (quote_checker(str, state) == -1)
			return (-1);
		if (!state->inside_quotes)
		{
			if (!skip_spaces(str, state))
				return (0); //
			if (is_pipe(str[state->j]))
			{
				if (create_pipe_token(state) == -1)
					return (-1);
				//continue ;
			}
			if (is_redirect(str[state->j]))
			{
				if (handle_redirection_tok(state, str) == -1)
					return (-1);
				//continue ;
			}
			else
			{
				if (create_word_token(state, str) == -1)
					return (-1);
				//continue ;
			}
		}
		else
		{
			if (handle_quote_word(state, str, data) == -1)
				return (-1);
		}
		state->j++;
	}
	return (0);
}

t_token	**split_to_tokens(const char *input, t_data *data)
{
	t_tokenizer_state	state;
	t_token				**tokens;

	if (!input)
		return (NULL);
	tokens = ft_calloc(ft_strlen(input) + 1, sizeof(t_token *));
	if (!tokens)
		return (perror("calloc"), NULL);
	if (!init_state(&state, tokens))
		return (free(tokens), NULL);
	skip_quotes_and_spaces(input, &state);
	if (tokenize_loop(input, &state, data) == -1)
		return (NULL);
	if (state.inside_quotes)
	{
		write_error("minishell: syntax error: unclosed quotes\n");
		return (data->exit_status = 2, NULL);
	}
	// tokens[state.i] = NULL;
	free(state.buffer);
	debug_print_tokens(tokens);
	return (tokens);
}
