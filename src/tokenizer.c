//split to token 
#include "../minishell.h"

t_token_type classify_token(const char *str)
{
    if (strcmp(str, "|") == 0)
        return (PIPE);
    else if (strcmp(str, "<") == 0)
        return (REDIRECT_IN);
    else if (strcmp(str, ">") == 0)
        return (REDIRECT_OUT);
    else if (strcmp(str, ">>") == 0)
        return (APPEND);
    else if (strcmp(str, "<<") == 0)
        return (HEREDOC);
    else
        return (WORD);
}

t_token	split_to_token(char *str)
{
	t_token **tokens;
	char *token_str;
    int i;

	tokens = ft_calloc(TOKEN_BUFFER_SIZE + 1, sizeof(t_token *));
    if (!tokens)
	{
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
	token_str = ft_strtok(str, " \t\n"); // \r
	i = 0;
    while (token_str)
	{
        //tokens[i++] = ft_strdup(token->value);
		//printf("%s\n", tokens[i - 1]);
        if (i >= TOKEN_BUFFER_SIZE)
		{
            perror("Token buffer overflow");
            exit(EXIT_FAILURE);
        }
 		tokens[i] = ft_calloc(sizeof(t_token), 1);
        if (!tokens[i])
		{
            perror("Malloc failed");
			free_tokens(tokens);
            exit(EXIT_FAILURE);
        }
        tokens[i]->type = classify_token(token_str);
        tokens[i]->value = ft_strdup(token_str);
        token_str = ft_strtok(NULL, " \t\n");
        i++;
    }
	// print
	for (int i = 0; tokens[i] != NULL; i++)
		printf("Token[%d]: Type: %d, Value: %s\n", i, tokens[i]->type, tokens[i]->value);
    tokens[i] = NULL;
	//free_tokens(tokens);
    return (**tokens);
}
