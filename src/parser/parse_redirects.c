/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:15:32 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/12 18:48:20 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	initialize_redirect_array(char ***redirects, const char *value,
		bool **flags, bool flag)
{
	*redirects = ft_calloc(2, sizeof(char *));
	if (!*redirects)
		return (perror("ft_calloc"), ERROR);
	if (flags)
	{
		*flags = ft_calloc(2, sizeof(bool));
		if (!*flags)
			return (free(*redirects), *redirects = NULL, perror("ft_calloc"),
				ERROR);
	}
	(*redirects)[0] = ft_strdup(value);
	if (!(*redirects)[0])
	{
		if (flags && *flags)
			free(*flags);
		if (flags)
			*flags = NULL;
		return (free(*redirects), *redirects = NULL, perror("strdup"), ERROR);
	}
	if (flags)
	{
		(*flags)[0] = flag;
		(*flags)[1] = false;
	}
	return ((*redirects)[1] = NULL, SUCCESS);
}

int	append_redirect_value(char ***redirects, const char *value, bool **flags,
		bool flag)
{
	t_redirect_append	data;
	bool				*flags_ptr;

	if (init_append_data(&data, *redirects, flags) == ERROR)
		return (ERROR);
	flags_ptr = NULL;
	if (flags)
		flags_ptr = *flags;
	copy_existing_data(&data, *redirects, flags_ptr);
	if (add_new_value(&data, value, flag) == ERROR)
	{
		if (data.has_flags)
			free(data.new_flags);
		free(data.new_array);
		return (ERROR);
	}
	if (data.has_flags)
	{
		free(*flags);
		*flags = data.new_flags;
	}
	free(*redirects);
	*redirects = data.new_array;
	return (SUCCESS);
}

int	parse_redirects(t_cmd *cmd, t_token *token, t_token_type type)
{
	char	***redirects;
	bool	**flags;

	redirects = get_redirect_target(cmd, type);
	flags = get_redirect_flag_target(cmd, type);
	if (!redirects)
		return (SUCCESS);
	if (!*redirects)
		return (initialize_redirect_array(redirects, token->value, flags,
				token->touch_quotes));
	else
		return (append_redirect_value(redirects, token->value, flags,
				token->touch_quotes));
}

int	handle_redirection_parser(t_cmd *cmd, t_token **tokens, t_data *data,
		int *i)
{
	char	*unexpected_token;

	if (!tokens[*i + 1])
		return (write_error("minishell: syntax error near unexpected"),
			write_error(" token `newline'\n"), data->exit_status = 20);
	if (tokens[*i + 1]->type == NOTHING)
		return (0);
	if (tokens[*i + 1]->type != WORD)
	{
		unexpected_token = tokens[*i + 1]->value;
		write_error("minishell: syntax error near unexpected token `%s'\n",
			unexpected_token);
		data->exit_status = 2;
		return (0);
	}
	if (!parse_redirects(cmd, tokens[*i + 1], tokens[*i]->type))
		return (0);
	(*i)++;
	return (1);
}
