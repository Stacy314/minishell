/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 20:33:52 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/07 21:28:27 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_shlvl_line(char **line)
{
	int		shlvl;
	char	*tmp;
	char	*new;

	shlvl = ft_atoi(*line + 6) + 1;
	tmp = ft_itoa(shlvl);
	if (!tmp)
		return (ERROR);
	new = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	if (!new)
		return (ERROR);
	free(*line);
	*line = new;
	return (SUCCESS);
}

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
