/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/13 00:36:19 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_variable(const char *str, int *j, t_data *data)
{
	char	*var_name;
	char	*value;

	(*j)++;
	if (str[*j] == '?')
		return (get_exit_status_value(data, j));
	var_name = extract_var_name(str, j);
	if (!var_name)
		return (NULL);
	if (var_name[0] == '$' && var_name[1] == '\0')
		return (var_name);
	value = search_env_value(var_name, data);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (value);
}


int	expand_buffer(t_tokenizer_state *state)
{
	char	*new_buffer;

	state->buffer_size *= 2;
	new_buffer = ft_calloc(state->buffer_size, sizeof(char));
	if (!new_buffer)
		return (perror("calloc"), -1);
	ft_strlcpy(new_buffer, state->buffer, state->buffer_size);
	free(state->buffer);
	state->buffer = new_buffer;
	return (0);
}

static int	handle_single_quote_expansion(t_tokenizer_state *state,
	const char *str, int start, char *expanded)
{
	int	i;

	i = start;
	while (i < state->j)
		state->buffer[state->k++] = str[i++];
	free(expanded);
	return (1);
}

static int	handle_expansion_error(t_tokenizer_state *state, char *expanded)
{
	free(expanded);
	if (state->quote_type == '\"')
		return (2);
	return (3);
}

int	handle_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	char	*expanded;
	size_t	len;
	int		start;

	if (str[state->j] != '$')
		return (0);
	start = state->j;
	expanded = expand_variable(str, &state->j, data);
	if (state->quote_type == '\'')
		return (handle_single_quote_expansion(state, str, start, expanded));
	if (!expanded)
		return (handle_expansion_error(state, expanded));
	len = ft_strlen(expanded);
	while (state->k + len >= (size_t)state->buffer_size)
	{
		if (expand_buffer(state) == -1)
			return (free(expanded), -1);
	}
	ft_strlcpy(&state->buffer[state->k], expanded,
		state->buffer_size - state->k);
	state->k += len;
	free(expanded);
	return (1);
}
