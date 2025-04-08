/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:15:32 by mgallyam          #+#    #+#             */
/*   Updated: 2025/04/08 16:04:57 by apechkov         ###   ########.fr       */
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
	{
		perror("ft_calloc");
		return (ERROR);
	}
	*flags = ft_calloc(2, sizeof(bool));
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
	(*flags)[0] = flag;
	return (SUCCESS);
}

int	append_redirect_value(char ***redirects, const char *value, bool **flags,
		bool flag)
{
	int		i;
	char	**new_array;
	bool	*new_flags;

	i = 0;
	while ((*redirects)[i])
		i++;
	new_array = ft_calloc(i + 2, sizeof(char *));
	new_flags = ft_calloc(i + 2, sizeof(bool));
	if (!new_array || !new_flags)
		return (ERROR);
	for (int j = 0; j < i; j++)
	{
		new_array[j] = (*redirects)[j];
		new_flags[j] = (*flags)[j];
	}
	new_array[i] = ft_strdup(value);
	new_flags[i] = flag;
	new_array[i + 1] = NULL;
	free(*redirects);
	free(*flags);
	*redirects = new_array;
	*flags = new_flags;
	return (SUCCESS);
}

int	parse_redirects(t_cmd *cmd, t_token *token, t_token_type type)
{
	char	***redirects;

	redirects = get_redirect_target(cmd, type);
	if (!redirects)
		return (SUCCESS);
	if (!*redirects)
		return (initialize_redirect_array(redirects, token->value,
				&cmd->heredoc_touch_quotes, token->touch_quotes));
	else
		return (append_redirect_value(redirects, token->value,
				&cmd->heredoc_touch_quotes, token->touch_quotes));
}

int	handle_redirection_parser(t_cmd *cmd, t_token **tokens, t_data *data,
		int *i)
{
	char	*unexpected_token;

	// if (tokens[*i + 1] && tokens[*i + 1]->type == NOTHING)
	//{
	//	data->exit_status = 1;
	//	write_error("minishell: ambiguous redirect\n"); // need add name on var
	//	return (0);
	//}
	if (!tokens[*i + 1])
	{
		
		return (0);
	}
	if (tokens[*i + 1]->type == NOTHING)
	{
		data->exit_status = 1;
		return (0); 
	}
	if (tokens[*i + 1]->type != WORD)
	{
		unexpected_token = tokens[*i + 1]->value;
		write_error("minishell: syntax error near unexpected token `%s'\n",
			unexpected_token);
		data->exit_status = 2;
		return (0);
	}

	
	//if (!tokens[*i + 1] || tokens[*i + 1]->type != WORD)
	//{
	//	if (tokens[*i + 1])
	//		unexpected_token = tokens[*i + 1]->value;
	//	else
	//		unexpected_token = "newline";
	//	write_error("minishell: syntax error near unexpected token `%s'\n",
	//		unexpected_token);
	//	data->exit_status = 2;
	//	return (0);
	//}
	if (data->heredoc_count > HEREDOC_MAX)
	{
		printf("heredoc count: %d\n", data->heredoc_count);
		write_error("minishell: maximum here-document count exceeded\n");
		data->exit_status = 2;
		return (0);
	}
	if (!parse_redirects(cmd, tokens[*i + 1], tokens[*i]->type))
		return (0);
	(*i)++;
	return (1);
}
