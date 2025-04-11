/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:44:45 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/10 16:46:53 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_quotes_and_redirects(t_tokenizer_state *state, const char *str,
	t_data *data)
{
	int	result;

	result = handle_quotes_and_redirects(state, str, data);
	if (result == MALLOC_ERROR || result == 2)
		return (result);
	if (result == 1)
		return (1);
	return (0);
}

int	check_expansion_result(int result, t_tokenizer_state *state,
	const char *str)
{
	if (result == MALLOC_ERROR)
		return (MALLOC_ERROR);
	if (result == 1 || result == 2)
		return (1);
	if (result == 3)
	{
		if (create_nothing_token(str, state) == MALLOC_ERROR)
			return (MALLOC_ERROR);
		return (1);
	}
	if (result == 2)
		return (2);
	return (0);
}
