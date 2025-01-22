#include "../minishell.h"

//need to fix: 
//			; (segfault)
//               ; (segfault)
//"" (should be empty arg)
//" " (should be empty arg)
//| echo hi  (bash: syntax error near unexpected token `|')
//echo hi | echo hi | (error with pipe, in bash opened input)
//echo "" (should print a new line)
//exit 1111111111111111111111111111111111 (protect from overflow, return value 1 and exit,
//		in bash - bash: exit: 1111111111111111111111111111111111: numeric argument required?)
//exit asdf (in bash - bash: exit: asdf: numeric argument required, exit and return value 2)
//exit 123 asdf (in bash - bash: exit: too many arguments and didn't exit, exit and return value 1?)
//echo $SHLVL (parse of env, also with "" and '')
//echo "" "        h           a           " (should print         h           a           ),
//		but this is right (echo          h           a)
//"echo hi" and 'echo hi' (should pars as one arg, echo hi: command not found)
//"echo" hi (should print hi)
//"ls" and 'ls' (should work as regular ls)
//e"ch"o hi (should print hi)
//'echo' hi (should print hi)
//echo hi '$USER is great, home is $HOME' (hi $USER is great, home is $HOME)
//echo hi "$USER is great, home is $HOME" (hi apechkov is great, home is /home/apechkov)

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

t_cmd *parse_tokens(t_token **tokens)
{
	t_cmd	*cmd;
	int			cmd_count;
	int			i;

    cmd = malloc(sizeof(t_cmd) * MAX_COMMANDS);
    if (!cmd)
    {
        perror("malloc");
		//free(tokens);
        return NULL;
    }
    cmd_count = 0;
    i = 0;
    cmd[cmd_count] = (t_cmd){NULL, NULL, NULL, NULL, NULL, 0, 0, NULL};
    while (tokens[i])
    {
        if (tokens[i]->type == PIPE)
        {
            cmd_count++;
            cmd[cmd_count] = (t_cmd){NULL, NULL, NULL, NULL, NULL, 1, 0, NULL};
        }
        else if (tokens[i]->type == REDIRECT_IN || tokens[i]->type == REDIRECT_OUT ||
                 tokens[i]->type == APPEND || tokens[i]->type == HEREDOC)
        {
            if (tokens[i + 1] && tokens[i + 1]->type == WORD)
            {
                if (tokens[i]->type == REDIRECT_IN)
                    cmd[cmd_count].input_redirect = strdup(tokens[i + 1]->value);
                else if (tokens[i]->type == REDIRECT_OUT)
                    cmd[cmd_count].output_redirect = strdup(tokens[i + 1]->value);
                else if (tokens[i]->type == APPEND)
                    cmd[cmd_count].append_redirect = strdup(tokens[i + 1]->value);
                else if (tokens[i]->type == HEREDOC)
                    cmd[cmd_count].heredoc_delimiter = strdup(tokens[i + 1]->value);
                i++;
            }
            else
            {
                fprintf(stderr, "Syntax error near unexpected token `%s`\n", tokens[i]->value);
                free(cmd);
                return (NULL);
            }
        }
        else
        {
            cmd[cmd_count].args = append_to_args(cmd[cmd_count].args, tokens[i]->value);
        }
        i++;
    }

    return (cmd);
}
