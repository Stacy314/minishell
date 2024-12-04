//split to token 
#include "../minishell.h"

//// Helper function: Checks if a character is a delimiter
//static bool is_delimiter(char c, const char *delim) {
//    while (*delim) {
//        if (c == *delim) {
//            return true;
//        }
//        delim++;
//    }
//    return false;
//}

char *ft_strtok(char *str, const char *delim) {
    static char *remainder;
    char *token;

    if (str != NULL) {
        remainder = str;
    }

    if (remainder == NULL || *remainder == '\0') {
        return NULL;
    }

    // Skip leading delimiters
    while (ft_strchr(delim, *remainder)) {
        remainder++;
    }

    if (*remainder == '\0') {
        return NULL;
    }

    // Start of the token
    token = remainder;

    // Find the end of the token
    while (*remainder != '\0' && !ft_strchr(delim, *remainder)) {
        remainder++;
    }

    // Terminate the token and move past the delimiter
    if (*remainder != '\0') {
        *remainder = '\0';
        remainder++;
    }

    return token;
}


char **split_to_token(char *str)
{
	char **tokens = ft_calloc(TOKEN_BUFFER_SIZE * sizeof(char *), 1);
    int token_index = 0;
    char *token;
    
    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Use strtok for basic space-based splitting
    token = ft_strtok(str, " \t\r\n");
    while (token != NULL) {
        tokens[token_index++] = ft_strdup(token);
		//printf("%s\n", tokens[token_index - 1]);
        if (token_index >= TOKEN_BUFFER_SIZE) {
            perror("token buffer overflow");
            exit(EXIT_FAILURE);
        }
        token = ft_strtok(NULL, " \t\r\n");
    }
    tokens[token_index] = NULL; // Null-terminate the token array
    return (tokens);
}
