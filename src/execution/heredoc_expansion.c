/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/11 22:12:26 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	append_variable(t_expand_ctx *ctx, t_data *data)
{
	char	*var_value;

	var_value = get_env_heredoc(&ctx->line[ctx->i + 1], data);
	if (!var_value)
		return (free(ctx->result), 0);
	if (!ensure_buffer_capacity(ctx, ctx->k
			+ ft_strlen(var_value)))
		return (free(var_value), 0);
	ft_strlcpy(&(ctx->result)[ctx->k], var_value, ctx->buffer_size - ctx->k);
	ctx->k += ft_strlen(var_value);
	free(var_value);
	(ctx->i)++;
	while (ft_isalnum(ctx->line[ctx->i]) || ctx->line[ctx->i] == '_')
		(ctx->i)++;
	return (1);
}

static int	append_char(char c, t_expand_ctx *ctx)
{
	char	*tmp;

	if (ctx->k + 2 >= ctx->buffer_size)
	{
		tmp = ft_calloc(ctx->buffer_size * 2, sizeof(char));
		if (!tmp)
			return (free(ctx->result), perror("calloc"), 0);
		ft_strlcpy(tmp, ctx->result, ctx->buffer_size * 2);
		free(ctx->result);
		ctx->result = tmp;
		ctx->buffer_size *= 2;
	}
	(ctx->result)[(ctx->k)++] = c;
	return (1);
}

static int	append_exit_status(t_expand_ctx *ctx, t_data *data)
{
	char	*exit_str;

	exit_str = ft_itoa(data->exit_status);
	if (!exit_str)
		return (free(ctx->result), perror("itoa"), 0);
	if (!ensure_buffer_capacity(ctx, ctx->k
			+ ft_strlen(exit_str)))
		return (free(ctx->result), free(exit_str), 0);
	ft_strlcpy(&ctx->result[ctx->k], exit_str, ctx->buffer_size - ctx->k);
	ctx->k += ft_strlen(exit_str);
	free(exit_str);
	return (1);
}

char	*expand_loop(const char *line, t_data *data, t_expand_ctx *ctx)
{
	while (line[ctx->i])
	{
		if (line[ctx->i] == '$' && line[ctx->i + 1] == '?')
		{
			if (!append_exit_status(ctx, data))
				return (free(ctx), NULL);
			ctx->i += 2;
		}
		else if (line[ctx->i] == '$' && line[ctx->i + 1]
			&& (ft_isalnum(line[ctx->i + 1]) || line[ctx->i + 1] == '_'))
		{
			if (!append_variable(ctx, data))
				return (free(ctx->result), free(ctx), NULL);
		}
		else
		{
			if (!append_char(ctx->line[ctx->i], ctx))
				return (free(ctx->result), free(ctx), NULL);
		}
	}
	return (ctx->result);
}

char	*expand_heredoc(const char *line, t_data *data)
{
	t_expand_ctx	*ctx;

	ctx = NULL;
	if (!line)
		return (NULL);
	ctx = init_ctx(ctx, line);
	if (!ctx)
		return (NULL);
	ctx->result = expand_loop(line, data, ctx);
	if (!ctx->result)
		return (NULL);
	return (ctx->result);
}
