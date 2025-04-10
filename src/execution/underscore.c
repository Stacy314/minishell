/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underscore.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 23:03:11 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_last_value(t_token **tokens)
{
	int		i;
	char	*last_value;

	last_value = NULL;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->value)
			last_value = tokens[i]->value;
		i++;
	}
	return (last_value);
}

void	update_underscore(t_data *data, char *value)
{
	int		i;
	char	*new_val;

	if (!value)
		return ;
	new_val = ft_strjoin("_=", value);
	if (!new_val)
		return ;
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], "_=", 2) == 0)
		{
			free(data->env[i]);
			data->env[i] = new_val;
			return ;
		}
		i++;
	}
	add_or_update_env(new_val, data);
}
