/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 23:10:51 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_variable(const char *str, int *j, t_data *data)
{
	char	*var_name;
	int		k;
	char	**env;
	int		i;
	int		start;
	char	*tmp;

	k = 0;
	i = 0;
	(*j)++;
	if (str[*j] == '?')
		return ((*j)++, ft_itoa(data->exit_status));
	start = *j;
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		(*j)++;
	if (*j == start)
	{
		tmp = ft_calloc(2, sizeof(char));
		if (!tmp)
			return (perror("calloc1"), NULL);
		tmp[0] = '$';
		return (tmp);
	}
	var_name = ft_calloc((*j - start + 1), 1);
	if (!var_name)
		return (perror("calloc2"), NULL);
	while (start < *j)
		var_name[k++] = str[start++];
	var_name[k] = '\0';
	env = data->env;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, k) && env[i][k] == '=')
		{
			free(var_name);
			return (ft_strdup(env[i] + k + 1));
		}
		i++;
	}
	free(var_name);
	return (NULL);
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

int	handle_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	char	*expanded;
	size_t	len;
	int		start_j;
	int		i;

	if (str[state->j] == '$')
	{
		start_j = state->j;
		expanded = expand_variable(str, &state->j, data);
		if (state->quote_type == '\'')
		{
			i = start_j;
			while (i < state->j)
				state->buffer[state->k++] = str[i++];
			return (free(expanded), 1);
		}
		if (!expanded)
		{
			free(expanded);
			if (state->quote_type == '\"')
				return (free(expanded), 2);
			return (free(expanded), 3);
		}
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
