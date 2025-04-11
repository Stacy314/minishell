/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:55:27 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/11 23:11:58 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	init_append_data(t_redirect_append *data, char **redirects, bool **flags)
{
	data->size = 0;
	data->new_array = NULL;
	data->new_flags = NULL;
	data->has_flags = (flags != NULL);
	while (redirects[data->size])
		data->size++;
	data->new_array = ft_calloc(data->size + 2, sizeof(char *));
	if (!data->new_array)
		return (ERROR);
	if (data->has_flags)
	{
		data->new_flags = ft_calloc(data->size + 2, sizeof(bool));
		if (!data->new_flags)
		{
			free(data->new_array);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

int	copy_existing_data(t_redirect_append *data, char **redirects, bool *flags)
{
	int	j;

	j = 0;
	while (j < data->size)
	{
		data->new_array[j] = redirects[j];
		if (data->has_flags)
			data->new_flags[j] = flags[j];
		j++;
	}
	return (SUCCESS);
}

int	add_new_value(t_redirect_append *data, const char *value, bool flag)
{
	data->new_array[data->size] = ft_strdup(value);
	if (!data->new_array[data->size])
		return (ERROR);
	data->new_array[data->size + 1] = NULL;
	if (data->has_flags)
		data->new_flags[data->size] = flag;
	return (SUCCESS);
}
