/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/27 17:12:20 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_spaces(const char *str, t_tokenizer_state *state)
{
	while (str[state->j] && (str[state->j] == ' ' || str[state->j] == '\t' || str[state->j] == '\n'))
		state->j++;
}

//int	is_logical_operator(const char *str, t_tokenizer_state *state)
//{
//	if (str[state->j] == '&' && str[state->j + 1] == '&')
//	{
//		flush_buffer_to_token(state);
//		state->tokens[state->i++] = create_token("&&", LOGICAL_AND,
//				state->index++);
//		state->j += 2;
//		return (1);
//	}
//	if (str[state->j] == '|' && str[state->j + 1] == '|')
//	{
//		flush_buffer_to_token(state);
//		state->tokens[state->i++] = create_token("||", LOGICAL_OR,
//				state->index++);
//		state->j += 2;
//		return (1);
//	}
//	return (0);
//}

int	is_pipe_operator(const char *str, t_tokenizer_state *state)
{
	if (str[state->j] == '|')
	{
		flush_buffer_to_token(state);
		state->tokens[state->i++] = create_token("|", PIPE, state->index++);
		state->j++;
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
