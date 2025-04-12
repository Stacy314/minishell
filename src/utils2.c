/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 20:22:36 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_fd_main(t_data *data)
{
	if (STDIN_FILENO != -1)
		close(STDIN_FILENO);
	if (STDIN_FILENO != -1)
		close(STDOUT_FILENO);
	if (data->saved_stdin != -1)
		close(data->saved_stdin);
}

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
