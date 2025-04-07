/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:15:32 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/07 17:22:20 by mgallyam         ###   ########.fr       */
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

int	initialize_redirect_array(char ***redirects, const char *value)
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

int	append_redirect_value(char ***redirects, const char *value)
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

int	parse_redirects(t_cmd *cmd, t_token *token, t_token_type type)
{
	char	***redirects;

	redirects = get_redirect_target(cmd, type);
	if (!redirects)
		return (SUCCESS);
	if (!*redirects)
		return (initialize_redirect_array(redirects, token->value));
	else
		return (append_redirect_value(redirects, token->value));
}

int	handle_redirection_parser(t_cmd *cmd, t_token **tokens, t_data *data,
		int *i)
{
	char	*unexpected_token;

	if (tokens[*i + 1] && tokens[*i + 1]->type == NOTHING)
	{
		data->exit_status = 1;
		write_error("minishell: ambiguous redirect\n");
		return (0);
	}
	if (!tokens[*i + 1] || tokens[*i + 1]->type != WORD)
	{
		if (tokens[*i + 1])
			unexpected_token = tokens[*i + 1]->value;
		else
			unexpected_token = "newline";
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
