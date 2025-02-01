/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/25 15:34:29 by apechkov         ###   ########.fr       */
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
    return new;
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

        // Проверяем кавычки
        if (str[j] == '\'' || str[j] == '\"')
        {
            char quote = str[j++];
            while (str[j] && str[j] != quote)
            {
                if (k < (int)(sizeof(buffer) - 1))
                    buffer[k++] = str[j++];
            }

            // Если кавычка не закрылась, выводим ошибку и выходим
            if (str[j] != quote)
            {
                fprintf(stderr, "minishell: syntax error: unclosed quotes\n");
                free(tokens);
                return (t_token **)(-1);
            }

            j++; // Пропускаем закрывающую кавычку
        }
        else
        {
            // Читаем обычное слово
            while (str[j] && !isspace((unsigned char)str[j]) && str[j] != '|' && str[j] != '<' && str[j] != '>')
            {
                if (k < (int)(sizeof(buffer) - 1))
                    buffer[k++] = str[j++];
            }
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

	//handel "" and '' (echo " | ")
	// print
	//for (int i = 0; tokens[i] != NULL; i++)
	//	printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type, tokens[i]->value);
    return (tokens);
}
