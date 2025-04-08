/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/08 22:45:53 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_spaces(const char *str, t_tokenizer_state *state)
{
	while (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t'
			|| str[state->j] == '\n'))
		state->j++;
}

int	create_pipe_operator(const char *str, t_tokenizer_state *state)
{
	if (str[state->j] == '|')
	{
		state->tokens[state->i++] = create_token("|", PIPE, state->index++,
				false);
		return (1);
	}
	return (0);
}

int	is_redirect(char c)
{
	return (c == '>' || c == '<');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_pipe(char c)
{
	return (c == '|');
}
