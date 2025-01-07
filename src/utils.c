#include "../minishell.h"
	
void free_tokens(t_token **tokens)
{
	int i;

	i = 0;
	if (!tokens)
        return;
   	while (tokens[i])
	{
        free(tokens[i]->value);
        free(tokens[i]);
		i++;
    }
    free(tokens);
}

char *ft_strtok(char *str, const char *delim)
{
    static char *remainder;
    char *token;

    if (str != NULL) {
        remainder = str;
    }

    if (remainder == NULL || *remainder == '\0') {
        return NULL;
    }
    while (ft_strchr(delim, *remainder)) {
        remainder++;
    }

    if (*remainder == '\0') {
        return NULL;
    }
    token = remainder;
    while (*remainder != '\0' && !ft_strchr(delim, *remainder)) {
        remainder++;
    }
    if (*remainder != '\0') {
        *remainder = '\0';
        remainder++;
    }
    return (token);
}
