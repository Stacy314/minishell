/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 01:16:05 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_env_heredoc(const char *var, t_data *data)
{
	int		i;
	int		len;
	char	*name;
	char	*value;

	len = 0;
	while (var[len] && (ft_isalnum(var[len]) || var[len] == '_'))
		len++;
	name = ft_substr(var, 0, len);
	if (!name)
		return (NULL);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name, len) == 0
			&& data->env[i][len] == '=')
		{
			value = ft_strdup(data->env[i] + len + 1);
			free(name);
			return (value);
		}
		i++;
	}
	free(name);
	return (ft_strdup(""));
}

int	ensure_buffer_capacity(t_expand_ctx *ctx, size_t required_size)
{
	char	*tmp;

	while (required_size >= ctx->buffer_size - 1)
	{
		tmp = ft_calloc(ctx->buffer_size * 2, sizeof(char));
		if (!tmp)
			return (perror("calloc"), 0);
		ft_strlcpy(tmp, ctx->result, ctx->buffer_size * 2);
		free(ctx->result);
		ctx->result = tmp;
		ctx->buffer_size *= 2;
	}
	return (1);
}
