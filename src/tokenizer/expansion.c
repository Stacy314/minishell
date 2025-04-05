/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 20:13:28 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// > $notexists echo "haha" (bash: $notexists: ambiguous redirect, exit code 1)

// cat <<HERE
// $USER
// oi
// HERE

// delimiter should be $USER
// 	cat << $USER

char	*expand_variable(const char *str, int *j, t_data *data)
{
	char	var_name[256];
	int		k;
	char	**env;
	int		i;

	k = 0;
	(*j)++;
	if (str[*j] == '?')
	{
		(*j)++;
		return (ft_itoa(data->exit_status));
	}
	while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
	{
		if (k < 255)
			var_name[k++] = str[*j];
		(*j)++;
	}
	var_name[k] = '\0';
	if (k == 0)
		return (ft_strdup("$"));
	env = data->env;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, k) && env[i][k] == '=')
			return (ft_strdup(env[i] + k + 1));
		i++;
	}
	return (ft_strdup(""));
}

int	expand_buffer(t_tokenizer_state *state)
{
	char	*new_buffer;

	state->buffer_size *= 2;
	new_buffer = ft_calloc(state->buffer_size, sizeof(char));
	if (!new_buffer)
		return (-1);
	ft_strlcpy(new_buffer, state->buffer, state->buffer_size);
	free(state->buffer);
	state->buffer = new_buffer;
	return (0);
}

int	handle_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	char	*expanded;
	size_t	len;

	if (str[state->j] == '$' && state->quote_type != '\'')
	{
		expanded = expand_variable(str, &state->j, data);
		if (!expanded || !*expanded)
			return (2);
		len = ft_strlen(expanded);
		while (state->k + len >= (size_t)state->buffer_size)
		{
			if (expand_buffer(state) == -1)
			{
				printf("expand_buffer failed\n");
				return (free(expanded), -1);
			}
		}
		ft_strlcpy(&state->buffer[state->k], expanded, state->buffer_size
			- state->k);
		state->k += len;
		free(expanded);
		return (1);
	}
	return (0);
}
