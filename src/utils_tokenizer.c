/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/18 22:13:24 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int is_redirect(char c)
{
	return (c == '>' || c == '<');
}

int expand_buffer(t_tokenizer_state *state)
{
	char *new_buffer;

	state->buffer_size *= 2;
	new_buffer = ft_calloc(state->buffer_size, sizeof(char));
	if (!new_buffer)
		return (-1);
	ft_strlcpy(new_buffer, state->buffer, state->buffer_size);
	free(state->buffer);
	state->buffer = new_buffer;
	return (0);
}

void *append_char_to_buffer(t_tokenizer_state *state, char c)
{
	if (state->k >= state->buffer_size - 1)
	{
		if (expand_buffer(state) == -1)
		{
			perror("expand_buffer");
			exit(EXIT_FAILURE);
		}
	}
	state->buffer[state->k++] = c;
	state->buffer[state->k] = '\0';
}

void	free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
