/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/17 23:11:49 by apechkov         ###   ########.fr       */
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
		exit(EXIT_FAILURE);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		perror("strdup");
		free(new);
		exit(EXIT_FAILURE);
	}
	new->type = type;
	new->index = index;
	return (new);
}

int	handle_redirection(const char *str, int j, t_token **tokens, int *i,
		int *index)
{
	if (str[j] == '>' || str[j] == '<')
	{
		if (str[j] == '>' && str[j + 1] == '>')
		{
			tokens[*i] = create_token(">>", APPEND, (*index)++);
			j += 2;
		}
		else if (str[j] == '<' && str[j + 1] == '<')
		{
			tokens[*i] = create_token("<<", HEREDOC, (*index)++);
			j += 2;
		}
		else if (str[j] == '>')
		{
			tokens[*i] = create_token(">", REDIRECT_OUT, (*index)++);
			j++;
		}
		else if (str[j] == '<')
		{
			tokens[*i] = create_token("<", REDIRECT_IN, (*index)++);
			j++;
		}
		(*i)++;
	}
	return (j);
}

t_token	**split_to_tokens(const char *str, t_data *data)
{
	t_token	**tokens;
	int		k;
	int		inside_quotes;
	char	quote_type;
	size_t	len;
	char	*expanded;

	char buffer[1024];
		// realloc wrapper function to continuously increase size for every additional export
	int capacity, i, j, index;
	if (!str || ft_strlen(str) == 0)
		return (NULL);
	capacity = ft_strlen(str) + 1;
	tokens = ft_calloc(capacity, sizeof(t_token *));
	if (!tokens)
	{
		perror("calloc");
		return (NULL);
	}
	i = 0;
	j = 0;
	index = 0;
	while (str[j])
	{
		while (str[j] && (str[j] == ' ' || str[j] == '	'))
			j++;
		if (str[j] == '\0')
			break ;
		k = 0;
		inside_quotes = 0;
		quote_type = 0;
		if (str[j] == '&' && str[j + 1] == '&')
		{
			tokens[i++] = create_token("&&", LOGICAL_AND, index++);
			j += 2;
			continue ;
		}
		if (str[j] == '|' && str[j + 1] == '|')
		{
			tokens[i++] = create_token("||", LOGICAL_OR, index++);
			j += 2;
			continue ;
		}
		if (str[j] == '|')
		{
			tokens[i] = create_token("|", PIPE, index++);
			i++;
			j++;
			continue ;
		}
		if (!inside_quotes && (str[j] == '>' || str[j] == '<'))
		{
			j = handle_redirection(str, j, tokens, &i, &index);
			continue ;
		}
		while (str[j] && (!ft_isspace((unsigned char)str[j]) || inside_quotes))
		{
			if (inside_quotes && str[j] == '|')
			{
				buffer[k++] = str[j++];
				continue ;
			}
			if ((str[j] == '\'' || str[j] == '\"') && (!inside_quotes
					|| str[j] == quote_type))
			{
				if (!inside_quotes)
				{
					inside_quotes = 1;
					quote_type = str[j];
				}
				else if (inside_quotes && str[j] == quote_type)
				{
					inside_quotes = 0;
					quote_type = 0;
				}
				j++;
				continue ;
			}
			if (!inside_quotes && (str[j] == '>' || str[j] == '<'))
			{
				if (k > 0)
				{
					buffer[k] = '\0';
					tokens[i++] = create_token(buffer, WORD, index++);
					k = 0;
				}
				j = handle_redirection(str, j, tokens, &i, &index);
				continue ;
			}
			if (str[j] == '$' && quote_type != '\'')
			{
				expanded = expand_variable(str, &j, data);
				// move to parser
				if (!expanded || !*expanded)  // If empty after expansion
				{
				//	printf("minishell: %s: ambiguous redirect\n",
				// cmd->output_redirect); //change to stderr
					data->exit_status = 1;
					free(expanded);
					return (NULL);
				}
				len = ft_strlen(expanded);
				if (k + len < sizeof(buffer))
				{
					ft_strlcpy(&buffer[k], expanded, sizeof(buffer) - k);
					k += len;
				}
				free(expanded);
				continue ;
			}
			if (k < (int)(sizeof(buffer) - 1))
				buffer[k++] = str[j++];
		}
		if (inside_quotes)
		{
			write_error("minishell: syntax error: unclosed quotes\n");
			free_tokens(tokens);
			return (t_token **)(-1);
		}
		buffer[k] = '\0';
		if (buffer[0] != '\0')
			tokens[i++] = create_token(buffer, WORD, index++);
	}
	if (i == 0)
	{
		free(tokens);
		return (NULL);
	}
	tokens[i] = NULL;
	// Debug print
	// for (int i = 0; tokens[i] != NULL; i++)
	//   printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type,
	// 	tokens[i]->value);
	return (tokens);
}
