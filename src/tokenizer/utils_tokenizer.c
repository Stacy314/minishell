/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/05 23:50:56 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	skip_spaces(const char *str, t_tokenizer_state *state)
{
	while (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t'
			|| str[state->j] == '\n'))
		state->j++;
	if (str[state->j] == '\0')
		return (0);
	return (1);
}

void	skip_quotes_and_spaces(const char *str, t_tokenizer_state *state)
{
	while (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t'
			|| str[state->j] == '\n' || is_quote(str[state->j])))
	{
		if (str[state->j] == ' ' || str[state->j] == '\t' || str[state->j] == '\n')
			state->j++;
		if (is_quote(str[state->j]))
		{
			if (str[state->j] == '\'' || str[state->j] == '\"')
			{
				if (state->inside_quotes)
					state->inside_quotes = 0;
				else
					state->inside_quotes = 1;
				state->quote_type = str[state->j];
				state->j++;
			}
			if ((str[state->j] == '\'' || str[state->j] == '\"')
					&& state->inside_quotes)
			{
				create_nothing_token(str, state);
					//return (-1);
				state->inside_quotes = 0;
			}
			//return (0);
		}

		
			
	}

}