#include "../minishell.h"

char **append_to_args(char **args, char *new_arg)
{
    int		len;
	char	**new_args;
	int		i;

	len = 0;
    if (args)
    {
        while (args[len])
            len++;
    }
    new_args = malloc(sizeof(char *) * (len + 2));
    if (!new_args)
    {
        perror("malloc");
		//free(args);
        return (NULL);
    }
    i = 0;
    while (i < len)
    {
        new_args[i] = args[i];
        i++;
    }
    new_args[len] = strdup(new_arg);
    new_args[len + 1] = NULL;
    free(args);
    return (new_args);
}

t_command *parse_tokens(t_token **tokens)
{
	t_command	*commands;
	int			cmd_count;
	int			i;

    commands = malloc(sizeof(t_command) * MAX_COMMANDS);
    if (!commands)
    {
        perror("malloc");
		//free(tokens);
        return NULL;
    }
    cmd_count = 0;
    i = 0;
    commands[cmd_count] = (t_command){NULL, NULL, NULL, NULL, NULL, 0, 0};
    while (tokens[i])
    {
        if (tokens[i]->type == PIPE)
        {
            cmd_count++;
            commands[cmd_count] = (t_command){NULL, NULL, NULL, NULL, NULL, 1, 0};
        }
        else if (tokens[i]->type == REDIRECT_IN || tokens[i]->type == REDIRECT_OUT ||
                 tokens[i]->type == APPEND || tokens[i]->type == HEREDOC)
        {
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
                i++;
            }
            else
            {
                fprintf(stderr, "Syntax error near unexpected token `%s`\n", tokens[i]->value);
                free(commands);
                return (NULL);
            }
        }
        else
        {
            commands[cmd_count].args = append_to_args(commands[cmd_count].args, tokens[i]->value);
        }
        i++;
    }

    return (commands);
}
