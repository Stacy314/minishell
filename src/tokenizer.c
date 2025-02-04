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
    int capacity, i, j, index;

    if (!str || ft_strlen(str) == 0)
        return NULL;

    capacity = ft_strlen(str) + 1;
    tokens = ft_calloc(capacity, sizeof(t_token *));
    if (!tokens)
    {
        perror("calloc");
        return NULL;
    }

    i = 0;
    j = 0;
    index = 0;

    while (str[j])
    {
        while (str[j] && str[j] == ' ')
            j++;
        if (str[j] == '\0')
            break;

        char buffer[1024];
        int k = 0;
        int inside_quotes = 0;
        char quote_type = 0;

        while (str[j] && (!isspace((unsigned char)str[j]) || inside_quotes))
        {
            if ((str[j] == '\'' || str[j] == '\"') && (!inside_quotes || str[j] == quote_type))
            {
                if (!inside_quotes)
                {
                    inside_quotes = 1;
                    quote_type = str[j]; // Запоминаем тип кавычек
                }
                else if (inside_quotes && str[j] == quote_type)
                {
                    inside_quotes = 0;
                    quote_type = 0; // Закрываем кавычки
                }
                j++; // Пропускаем кавычки
                continue;
            }

            if (k < (int)(sizeof(buffer) - 1))
                buffer[k++] = str[j++];
        }

        buffer[k] = '\0';
        if (buffer[0] != '\0')
            tokens[i++] = create_token(buffer, WORD, index++);
    }

    if (i == 0)
    {
        free(tokens);
        return NULL;
    }

    tokens[i] = NULL;

    // Debug print
    for (int i = 0; tokens[i] != NULL; i++)
        printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type, tokens[i]->value);
    
    return tokens;
}
