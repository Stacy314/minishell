/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_redirects.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:15:32 by mgallyam          #+#    #+#             */
/*   Updated: 2025/03/26 21:07:02 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char ***get_redirect_target(t_cmd *cmd, t_token_type type)
{
	if (type == REDIRECT_IN)
		return (&cmd->input_redirects);
	else if (type == REDIRECT_OUT)
		return (&cmd->output_redirects);
	else if (type == APPEND)
		return (&cmd->append_redirects);
	else if (type == HEREDOC)
		return (&cmd->heredoc_delimiter);
	return (NULL);
}

int initialize_redirect_array(char ***redirects, const char *value)
{
	*redirects = ft_calloc(2, sizeof(char *));
	if (!*redirects)
	{
		perror("ft_calloc");
		return (ERROR);
	}
	(*redirects)[0] = ft_strdup(value);
	if (!(*redirects)[0])
	{
		free(*redirects);
		perror("ft_strdup");
		return (ERROR);
	}
	(*redirects)[1] = NULL;
	return (SUCCESS);
}

int append_redirect_value(char ***redirects, const char *value)
{
	int		count;
	char	**new_array;
	int		i;

	count = 0;
	i = 0;
	while ((*redirects)[count])
		count++;
	new_array = ft_calloc(count + 2, sizeof(char *));
	if (!new_array)
	{
		perror("ft_calloc");
		return (ERROR);
	}
	while (i < count)
	{
		new_array[i] = (*redirects)[i];
		i++;
	}
	new_array[count] = ft_strdup(value);
	if (!new_array[count])
	{
		perror("ft_strdup");
		free(new_array);
		return (ERROR);
	}
	new_array[count + 1] = NULL;
	free(*redirects);
	*redirects = new_array;
	return (SUCCESS);
}
