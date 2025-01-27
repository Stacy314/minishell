/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/27 14:07:26 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *create_token(const char *value, t_token_type type, int index)
{
    t_token *new = ft_calloc(sizeof(t_token), 1);
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

t_token **split_to_tokens(const char *str)
{
    t_token **tokens;
    int      capacity;
    int      i;

    if (!str)
        return NULL;

    capacity = ft_strlen(str) + 1;
    tokens   = ft_calloc(capacity, sizeof(t_token *));
    if (!tokens)
    {
        perror("calloc");
        return NULL;
    }

    i = 0;
	int j = 0; 
    int index = 0;
    while (str[j])
    {
        while (str[j] && str[j] == ' ')
            j++;
        if (str[j] == '\0')
            break;
        if (str[j] == '|')
        {
            tokens[i] = create_token("|", PIPE, index++);
            i++;
            j++; 
            continue;
        }
		if (str[j] == '>')
        {
			if (str[j + 1] == '>')
			{
				tokens[i] = create_token(">>", APPEND, index++);
				i++;
				j += 2; 
				continue;
			}
			else
			{
				tokens[i] = create_token(">", REDIRECT_OUT, index++);
				i++;
				j++; 
				continue;
			}
        }
		if (str[j] == '<')
        {
			if (str[j + 1] == '<')
			{
				tokens[i] = create_token("<<", HEREDOC, index++);
				i++;
				j += 2; 
				continue;
			}
			else
			{
				tokens[i] = create_token("<", REDIRECT_IN, index++);
				i++;
				j++; 
				continue;
			}
        }

        {
            char   buffer[1024];
            size_t k = 0;
            while (str[j] && !isspace((unsigned char)str[j]) && str[j] != '|' && str[j] != '<' && str[j] != '>')
            {
                buffer[k++] = str[j];
                j++;
                if (k >= sizeof(buffer) - 1)
                    break;
            }
            buffer[k] = '\0';

            tokens[i] = create_token(buffer, WORD, index++);
            i++;
        }
    }
    tokens[i] = NULL;

	//handel "" and '' (echo " | ")
	// print
	//for (int i = 0; tokens[i] != NULL; i++)
	//	printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type, tokens[i]->value);
    return (tokens);
}
