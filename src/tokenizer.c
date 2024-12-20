#include "../minishell.h"

t_token_type classify_token(const char *str)
{
    if (ft_strncmp(str, "|", ft_strlen(str)) == 0)
        return (PIPE);
    else if (ft_strncmp(str, "<", ft_strlen(str)) == 0)
        return (REDIRECT_IN);
    else if (ft_strncmp(str, ">", ft_strlen(str)) == 0)
        return (REDIRECT_OUT);
    else if (ft_strncmp(str, ">>", ft_strlen(str)) == 0)
        return (APPEND);
    else if (ft_strncmp(str, "<<", ft_strlen(str)) == 0)
        return (HEREDOC);
    else
        return (WORD);
}
char *join_with_space(const char *s1, const char *s2)
{
	char *result;
	size_t len1;
	size_t len2;

    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    result = ft_calloc(len1 + len2 + 2, 1);
    if (!result)
	{
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
	ft_strlcpy(result, s1, len1 + 1);
    result[len1] = ' ';
    ft_strlcpy(result + len1 + 1, s2, len2 + 1);
    return (result);
}

t_token	**split_to_token(char *str)
{
	t_token **tokens;
	char *token_str;
	char *next_token;
    int i;
	int len;

	len = ft_strlen(str);
	tokens = ft_calloc(len + 1, sizeof(t_token *));
    if (!tokens)
	{
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
	token_str = ft_strtok(str, " \t\n\r");
	i = 0;
    while (token_str)
	{
 		tokens[i] = ft_calloc(sizeof(t_token), 1);
        if (!tokens[i])
		{
            perror("Malloc failed");
			free_tokens(tokens);
            exit(EXIT_FAILURE);
        }
        tokens[i]->type = classify_token(token_str);
		next_token = ft_strtok(NULL, " \t\n\r");
		if (tokens[i]->type == WORD && next_token && classify_token(next_token) == WORD)
		{   
			tokens[i]->value = join_with_space(token_str, next_token);
            token_str = ft_strtok(NULL, " \t\n\r");
		}
		else
		{
            tokens[i]->value = ft_strdup(token_str);
            token_str = next_token;
        }
		tokens[i]->index = i;
        i++;
    }
	tokens[i] = NULL;

	// print
	for (int i = 0; tokens[i] != NULL; i++)
		printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type, tokens[i]->value);
	//free_tokens(tokens);
    return (tokens);
}
