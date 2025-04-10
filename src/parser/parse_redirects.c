/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:15:32 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/10 23:09:36 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	***get_redirect_target(t_cmd *cmd, t_token_type type)
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
	int		i;
	int		j;
	char	**new_array;
	bool	*new_flags;

	new_flags = NULL;
	i = 0;
	while ((*redirects)[i])
		i++;
	new_array = ft_calloc(i + 2, sizeof(char *));
	if (!new_array)
		return (ERROR);
	if (flags)
	{
		new_flags = ft_calloc(i + 2, sizeof(bool));
		if (!new_flags)
			return (free(new_array), ERROR);
	}
	j = 0;
	while (j < i)
	{
		new_array[j] = (*redirects)[j];
		if (flags)
			new_flags[j] = (*flags)[j];
		j++;
	}
	new_array[i] = ft_strdup(value);
	if (!new_array[i])
	{
		if (flags)
			free(new_flags);
		return (free(new_array), ERROR);
	}
	new_array[i + 1] = NULL;
	if (flags)
	{
		new_flags[i] = flag;
		free(*flags);
		*flags = new_flags;
	}
	return (free(*redirects), *redirects = new_array, SUCCESS);
}

bool	**get_redirect_flag_target(t_cmd *cmd, t_token_type type)
{
	if (type == HEREDOC)
		return (&cmd->heredoc_touch_quotes);
	return (NULL);
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
	if (data->heredoc_count > HEREDOC_MAX)
		return (write_error("minishell: "),
			write_error("maximum here-document count exceeded\n"),
			data->exit_status = 2, 0);
	if (!parse_redirects(cmd, tokens[*i + 1], tokens[*i]->type))
		return (0);
	(*i)++;
	return (1);
}
