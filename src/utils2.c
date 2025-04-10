/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 22:15:13 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_copy(char **env_copy, int i)
{
	while (i-- > 0)
		free(env_copy[i]);
	free(env_copy);
}

int	prepare_input(t_data *data)
{
	int	ret;

	ret = read_line_and_check(data);
	if (ret != INPUT_SUCCESS)
		return (ret);
	ret = tokenize_and_parse(data);
	return (ret);
}
