#include "../minishell.h"

// Helper function to append an argument to the args array
char **append_to_args(char **args, char *new_arg)
{
    int len = 0;

    // Calculate the current length of args
    if (args)
    {
        while (args[len])
            len++;
    }

    // Reallocate memory for the new size (current length + 2)
    char **new_args = malloc(sizeof(char *) * (len + 2));
    if (!new_args)
    {
        perror("malloc");
        return NULL;
    }

    // Copy old arguments to the new array
    int i = 0;
    while (i < len)
    {
        new_args[i] = args[i];
        i++;
    }

    // Add the new argument and NULL-terminate the array
    new_args[len] = strdup(new_arg);
    new_args[len + 1] = NULL;

    // Free the old array if it exists
    free(args);
    return new_args;
}

// Main parsing function
// Converts tokens into a structured array of commands
// Each command includes its arguments and redirections

t_command *parse_tokens(t_token **tokens)
{
    // Allocate memory for commands
    t_command *commands = malloc(sizeof(t_command) * MAX_COMMANDS);
    if (!commands)
    {
        perror("malloc");
        return NULL;
    }

    // Initialize variables
    int cmd_count = 0; // Keeps track of the current command index
    int i = 0;         // Index for iterating over tokens

    // Initialize the first command
    commands[cmd_count] = (t_command){NULL, NULL, NULL, NULL, NULL, 0, 0};

    // Loop through tokens
    while (tokens[i])
    {
        // Handle pipes (|)
        if (tokens[i]->type == PIPE)
        {
            cmd_count++; // Start a new command

            // Initialize the new command structure
            commands[cmd_count] = (t_command){NULL, NULL, NULL, NULL, NULL, 1, 0};
        }
        // Handle redirections (<, >, >>, <<)
        else if (tokens[i]->type == REDIRECT_IN || tokens[i]->type == REDIRECT_OUT ||
                 tokens[i]->type == APPEND || tokens[i]->type == HEREDOC)
        {
            // Check that the next token exists and is a WORD (filename or delimiter)
            if (tokens[i + 1] && tokens[i + 1]->type == WORD)
            {
                if (tokens[i]->type == REDIRECT_IN)
                    commands[cmd_count].input_redirect = strdup(tokens[i + 1]->value);
                else if (tokens[i]->type == REDIRECT_OUT)
                    commands[cmd_count].output_redirect = strdup(tokens[i + 1]->value);
                else if (tokens[i]->type == APPEND)
                    commands[cmd_count].append_redirect = strdup(tokens[i + 1]->value);
                else if (tokens[i]->type == HEREDOC)
                    commands[cmd_count].heredoc_delimiter = strdup(tokens[i + 1]->value);

                i++; // Skip the next token since it's part of the redirection
            }
            else
            {
                // Syntax error if redirection is not followed by a valid filename/delimiter
                fprintf(stderr, "Syntax error near unexpected token `%s`\n", tokens[i]->value);
                free(commands);
                return NULL;
            }
        }
        // Handle normal words (commands or arguments)
        else
        {
            commands[cmd_count].args = append_to_args(commands[cmd_count].args, tokens[i]->value);
        }
        i++;
    }

    return commands;
}

// Test function for the parser
void test_parser(t_token **tokens)
{
    t_command *commands = parse_tokens(tokens);

    if (!commands)
    {
        fprintf(stderr, "Parsing failed!\n");
        return;
    }

    // Print parsed commands for debugging
    int i = 0;
    while (commands[i].args != NULL)
    {
        printf("Command %d:\n", i);
        printf("  Args: ");
        int j = 0;
        while (commands[i].args[j] != NULL)
        {
            printf("%s ", commands[i].args[j]);
            j++;
        }
        printf("\n");
        if (commands[i].input_redirect)
            printf("  Input Redirect: %s\n", commands[i].input_redirect);
        if (commands[i].output_redirect)
            printf("  Output Redirect: %s\n", commands[i].output_redirect);
        if (commands[i].append_redirect)
            printf("  Append Redirect: %s\n", commands[i].append_redirect);
        if (commands[i].heredoc_delimiter)
            printf("  Heredoc Delimiter: %s\n", commands[i].heredoc_delimiter);
        i++;
    }

    // Free allocated memory
    i = 0;
    while (commands[i].args != NULL)
    {
        int j = 0;
        while (commands[i].args[j] != NULL)
        {
            free(commands[i].args[j]);
            j++;
        }
        free(commands[i].args);
        free(commands[i].input_redirect);
        free(commands[i].output_redirect);
        free(commands[i].append_redirect);
        free(commands[i].heredoc_delimiter);
        i++;
    }
    free(commands);
}
