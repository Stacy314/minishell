/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tok.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 22:52:45 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	flush_word_before_redirect(t_tokenizer_state *state)
{
	state->buffer[state->k] = '\0';
	state->tokens[state->i] = create_token(state->buffer, WORD, state->index++);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->i++;
	state->k = 0;
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

int	create_pipe_token(t_tokenizer_state *state)
{
	state->tokens[state->i++] = create_token("|", PIPE, state->index++);
	if (!state->tokens[state->i - 1])
	{
		perror("failed create token");
		return (-1);
	}
	state->j++;
	return (1);
}

int	create_word_token(t_tokenizer_state *state, const char *str)
{
	int		start;
	int		len;
	char	*word;
	int		j;

	j = 0;
	start = state->j;
	len = 0;
	while (str[state->j] && !ft_isspace(str[state->j])
		&& !is_pipe(str[state->j]) && !is_redirect(str[state->j]) && !is_quote(str[state->j]))
	{
		state->j++;
		len++;
	}
	if (len == 0)
		return (0);
	word = ft_calloc((len + 1), 1);
	if (!word)
		return (-1);
	ft_strlcpy(word, str + start, len + 1);
	state->tokens[state->i] = create_token(word, WORD, state->index++);
	if (!state->tokens[state->i])
	{
		free(word);
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
		return (-1);
	}
	state->i++;
	free(word);
	return (1);
}

int	create_nothing_token(const char *str, t_tokenizer_state *state)
{
	state->tokens[state->i] = create_token("", EMPTY,
			state->index++);
	if (!state->tokens[state->i])
	{
		perror("failed create token");
		return (-1);
	}
	state->i++;
	state->j++;
	//if (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t'
	//	|| str[state->j] == '\n'))
	//	state->tokens[state->i]->spaces = 1;
	while (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t'
		|| str[state->j] == '\n'))
		state->j++;
	return (0);
}