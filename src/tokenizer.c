/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/18 22:21:12 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_token(const char *value, t_token_type type, int index)
{
	t_token	*new;

	new = ft_calloc(sizeof(t_token), 1);
	if (!new)
	{
		perror("calloc");
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		perror("strdup");
		free(new);
		return (NULL);
	}
	new->type = type;
	new->index = index;
	return (new);
}

int	handle_expansion(t_tokenizer_state *state, const char *str, t_data *data)
{
	char	*expanded;
	size_t	len;

	if (str[state->j] == '$' && state->quote_type != '\'')
	{
		expanded = expand_variable(str, &state->j, data);
		if (!expanded || !*expanded)
		{
			data->exit_status = 1;
			free(expanded);
			return (-1);
		}
		len = ft_strlen(expanded);
		while ((int)(state->k + len) >= state->buffer_size)
		{
			if (expand_buffer(state) == -1)
			{
				free(expanded);
				return (-1);
			}
		}
		ft_strlcpy(&state->buffer[state->k], expanded, state->buffer_size - state->k);
		state->k += len;
		free(expanded);
		state->j++;
		return (1);
	}
	return (0);
}

int	handle_redirection(t_tokenizer_state *state, const char *str)
{
	if (str[state->j] == '>' || str[state->j] == '<')
	{
		if (str[state->j] == '>' && str[state->j + 1] == '>')
		{
			state->tokens[state->i] = create_token(">>", APPEND, (state->index)++);
			if (!state->tokens[state->i])
			{
				perror("failed create token");
				return (-1);
			}
			state->j += 2;
		}
		else if (str[state->j] == '<' && str[state->j + 1] == '<')
		{
			state->tokens[state->i] = create_token("<<", HEREDOC, (state->index)++);
			if (!state->tokens[state->i])
			{
				perror("failed create token");
				return (-1);
			}
			state->j += 2;
		}
		else if (str[state->j] == '>')
		{
			state->tokens[state->i] = create_token(">", REDIRECT_OUT, (state->index)++);
			if (!state->tokens[state->i])
			{
				perror("failed create token");
				return (-1);
			}
			state->j++;
		}
		else if (str[state->j] == '<')
		{
			state->tokens[state->i] = create_token("<", REDIRECT_IN, (state->index)++);
			if (!state->tokens[state->i])
			{
				perror("failed create token");
				return (-1);
			}
			state->j++;
		}
		(state->i)++;
	}
	return (state->j);
}

int	handle_quotes_and_redirects(t_tokenizer_state *state, const char *str)
{
	if ((is_quote(str[state->j])) &&
		(!state->inside_quotes || str[state->j] == state->quote_type))
	{
		if (!state->inside_quotes)
		{
			state->inside_quotes = 1;
			state->quote_type = str[state->j];
		}
		else if (state->inside_quotes && str[state->j] == state->quote_type)
		{
			state->inside_quotes = 0;
			state->quote_type = 0;
		}
		state->j++;
		return (1);
	}
	if (!state->inside_quotes && is_redirect(str[state->j]))
	{
		if (state->k > 0)
		{
			state->buffer[state->k] = '\0';
			state->tokens[state->i] = create_token(state->buffer, WORD, state->index++);
			if (!state->tokens[state->i])
			{
				perror("failed create token");
				return (-1);
			}
			state->i++;
			state->k = 0;
		}
		if (handle_redirection(state, str) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

// t_token	**split_to_tokens(const char *str, t_data *data)
// {
// 	t_token	**tokens;
// 	int		k;
// 	int		inside_quotes;
// 	char	quote_type;
// 	size_t	len;
// 	char	*expanded;
// 	int		capacity;
// 	int		i;
// 	int		j;
// 	int		index;

// 	char buffer[1024]; // realloc wrapper function to continuously increase size for every additional export
// 	if (!str || ft_strlen(str) == 0)
// 		return (NULL);
// 	capacity = ft_strlen(str) + 1;
// 	tokens = ft_calloc(capacity, sizeof(t_token *));
// 	if (!tokens)
// 	{
// 		//free(buffer);
// 		perror("calloc");
// 		return (NULL);
// 	}
// 	i = 0;
// 	j = 0;
// 	index = 0;
// 	while (str[j])
// 	{
// 		while (str[j] && (str[j] == ' ' || str[j] == '	'))
// 			j++;
// 		if (str[j] == '\0')
// 			break ;
// 		k = 0;
// 		inside_quotes = 0;
// 		quote_type = 0;
// 		if (str[j] == '&' && str[j + 1] == '&')
// 		{
// 			tokens[i++] = create_token("&&", LOGICAL_AND, index++);
// 			if (!tokens[i])
// 			{
// 				perror("failed create token");
// 				return (NULL);
// 			}
// 			j += 2;
// 			continue ;
// 		}
// 		if (str[j] == '|' && str[j + 1] == '|')
// 		{
// 			tokens[i++] = create_token("||", LOGICAL_OR, index++);
// 			if (!tokens[i])
// 			{
// 				perror("failed create token");
// 				return (NULL);
// 			}
// 			j += 2;
// 			continue ;
// 		}
// 		if (str[j] == '|')
// 		{
// 			tokens[i] = create_token("|", PIPE, index++);
// 			if (!tokens[i])
// 			{
// 				perror("failed create token");
// 				return (NULL);
// 			}
// 			i++;
// 			j++;
// 			continue ;
// 		}
// 		if (!inside_quotes && (str[j] == '>' || str[j] == '<'))
// 		{
// 			j = handle_redirection(str, j, tokens, &i, &index);
// 			if (j == -1)
// 			{
// 				return (NULL);
// 			}
// 			continue ;
// 		}
// 		while (str[j] && (!ft_isspace((unsigned char)str[j]) || inside_quotes))
// 		{
// 			if (inside_quotes && str[j] == '|')
// 			{
// 				buffer[k++] = str[j++];
// 				continue ;
// 			}
// 			if ((str[j] == '\'' || str[j] == '\"') && (!inside_quotes
// 					|| str[j] == quote_type))
// 			{
// 				if (!inside_quotes)
// 				{
// 					inside_quotes = 1;
// 					quote_type = str[j];
// 				}
// 				else if (inside_quotes && str[j] == quote_type)
// 				{
// 					inside_quotes = 0;
// 					quote_type = 0;
// 				}
// 				j++;
// 				continue ;
// 			}
// 			if (!inside_quotes && (str[j] == '>' || str[j] == '<'))
// 			{
// 				if (k > 0)
// 				{
// 					buffer[k] = '\0';
// 					tokens[i++] = create_token(buffer, WORD, index++);
// 					if (!tokens[i])
// 					{
// 						perror("failed create token");
// 						return (NULL);
// 					}
// 					k = 0;
// 				}
// 				j = handle_redirection(str, j, tokens, &i, &index);
// 				continue ;
// 			}
// 			if (str[j] == '$' && quote_type != '\'')
// 			{
// 				expanded = expand_variable(str, &j, data);
// 				if (!expanded || !*expanded)
// 				{
// 					data->exit_status = 1;
// 					free(expanded);
// 					return (NULL);
// 				}
// 				len = ft_strlen(expanded);
// 				if (k + len < sizeof(buffer))
// 				{
// 					ft_strlcpy(&buffer[k], expanded, sizeof(buffer) - k);
// 					k += len;
// 				}
// 				free(expanded);
// 				continue ;
// 			}
// 			if (k < (int)(sizeof(buffer) - 1))
// 				buffer[k++] = str[j++];
// 		}
// 		if (inside_quotes)
// 		{
// 			write_error("minishell: syntax error: unclosed quotes\n");
// 			free_tokens(tokens);
// 			return (t_token **)(-1);
// 		}
// 		buffer[k] = '\0';
// 		if (buffer[0] != '\0')
// 			tokens[i++] = create_token(buffer, WORD, index++);
// 	}
// 	if (i == 0)
// 	{
// 		free(tokens);
// 		return (NULL);
// 	}
// 	tokens[i] = NULL;
// 	// Debug print
// 	// for (int i = 0; tokens[i] != NULL; i++)
// 	//   printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type,
// 	// 	tokens[i]->value);
// 	return (tokens);
// }

t_token	**split_to_tokens(const char *str, t_data *data)
{
	t_tokenizer_state	state;
	t_token	**tokens;
	int		result;

	result = 0;
	if (!str || ft_strlen(str) == 0)
		return (NULL);
	tokens = ft_calloc(ft_strlen(str) + 1, sizeof(t_token *));
	if (!tokens)
	{
		return (perror("calloc"), NULL);
	}
	initialize_state(&state, str, tokens);
	while (str[state.j])
	{
		while (str[state.j] && (str[state.j] == ' ' || str[state.j] == '	'))
			state.j++;
		if (str[state.j] == '\0')
			break ;
		if (str[state.j] == '&' && str[state.j + 1] == '&')
		{
			state.tokens[state.i++] = create_token("&&", LOGICAL_AND, state.index++);
			if (!tokens[state.i])
			{
				perror("failed create token");
				return (NULL);
			}
			state.j += 2;
			continue ;
		}
		if (str[state.j] == '|' && str[state.j + 1] == '|')
		{
			tokens[state.i++] = create_token("||", LOGICAL_OR, state.index++);
			if (!tokens[state.i])
			{
				perror("failed create token");
				return (NULL);
			}
			state.j += 2;
			continue ;
		}
		if (str[state.j] == '|')
		{
			tokens[state.i] = create_token("|", PIPE, state.index++);
			if (!tokens[state.i])
			{
				perror("failed create token");
				return (NULL);
			}
			state.i++;
			state.j++;
			continue ;
		}
		if (!state.inside_quotes && is_redirect(str[state.j]))
		{
			if (handle_redirection(&state, str) == -1)
			{
				free_tokens(tokens);
				free(state.buffer);
				return (NULL);
			}
			continue ;
		}
		while (str[state.j] && (!ft_isspace((unsigned char)str[state.j]) || state.inside_quotes))
		{
			if (state.inside_quotes && str[state.j] == '|')
			{
				state.buffer[state.k++] = str[state.j++];
				continue ;
			}
			int result = handle_quotes_and_redirects(&state, str);
			if (result == -1)
				return (free_tokens(tokens), free(state.buffer), NULL);
			if (result == 1)
				continue ;
			result = handle_expansion(&state, str, data);
			if (result == -1)
				return (free_tokens(tokens), free(state.buffer), NULL);
			if (result == 1)
				continue ;
			if (state.k < (int)(sizeof(state.buffer) - 1))
				state.buffer[state.k++] = str[state.j++];
		}
		if (state.inside_quotes)
		{
			write_error("minishell: syntax error: unclosed quotes\n");
			free_tokens(tokens);
			return (t_token **)(-1);
		}
		state.buffer[state.k] = '\0';
		if (state.buffer[0] != '\0')
			tokens[state.i++] = create_token(state.buffer, WORD, state.index++);
	}
	if (state.i == 0)
	{
		free(tokens);
		return (NULL);
	}
	tokens[state.i] = NULL;
	// Debug print
	// for (int i = 0; tokens[i] != NULL; i++)
	//   printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type,
	// 	tokens[i]->value);
	return (state.tokens);
}
