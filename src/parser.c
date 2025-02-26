#include "../minishell.h"

//need to fix:

//| echo hi (bash: syntax error near unexpected token `|', in our minishell -
//execve: No such file or directory + Conditional jump)

//echo hi | echo hi | (error with pipe, in our minishell - execve: No such file
//or directory)

//"echo hi" and 'echo hi' (should pars as one arg, echo hi: command not found)

//echo "|" echo (should print - | echo)

//echo " | "(should print -  | )

//echo hi>>4 >>5 >>6 (should create be 3 append's redirects)

//&& and || with parenthesis for priorities (no segfault, maybe print error)

//what will be in cmd if there are few redirects?


//fixed
//empty line (segfault), should print prompt again
//				(segfault) - tab
//                (segfault) - space
//"" (should be empty arg)
//" " (should be empty arg)
//echo "" (should print a new line)
//echo $SHLVL (parse of env, also with "" and '')
//echo "" "        h           a           " (should print         h           a           )
//"echo" hi (should print hi)
//"ls" and 'ls' (should work as regular ls)
//e"ch"o hi (should print hi)
//'echo' hi (should print hi)
//echo hi '$USER is great, home is $HOME' (hi $USER is great, home is $HOME)
//echo hi "$USER is great, home is $HOME" (hi apechkov is great, home is /home/apechkov)
//"<" sasdad
//<: command not found (in bash), in minishell - it is a command

void	print_cmds(t_cmd *cmd)
{
	int	i;
	int	cmd_index;

	cmd_index = 0;
	while (cmd)
	{
		printf("\nCommand[%d]:\n", cmd_index);
		// Печатаем аргументы команды
		printf("  Args: ");
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				printf("\"%s\" ", cmd->args[i]);
				i++;
			}
		}
		else
			printf("(null)");
		printf("\n");
		// Печатаем редиректы
		printf("  Input Redirect: %s\n",
				cmd->input_redirect ? cmd->input_redirect : "(null)");
		printf("  Output Redirect: %s\n",
				cmd->output_redirect ? cmd->output_redirect : "(null)");
		printf("  Append Redirect: %s\n",
				cmd->append_redirect ? cmd->append_redirect : "(null)");
		printf("  Heredoc Delimiter: %s\n",
				cmd->heredoc_delimiter ? cmd->heredoc_delimiter : "(null)");
		cmd = cmd->next;
		cmd_index++;
	}
}

char	**append_to_args(char **args, char *new_arg)
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

t_cmd	*parse_tokens(t_token **tokens)
{
	t_cmd	*cmd;
	int		cmd_count;
	int		i;

	cmd = malloc(sizeof(t_cmd) * MAX_COMMANDS);
	if (!cmd)
	{
		perror("malloc");
		//free(tokens);
		return (NULL);
	}
	cmd_count = 0;
	i = 0;
	cmd[cmd_count] = (t_cmd){NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	if (tokens[0]->type == PIPE)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
		free(cmd);
		return (NULL);
	}
	while (tokens[i])
	{
		if (tokens[i]->type == PIPE)
		{
			cmd_count++;
			cmd[cmd_count] = (t_cmd){NULL, NULL, NULL, NULL, NULL, NULL, NULL};
		}
		else if (tokens[i]->type == REDIRECT_IN
				|| tokens[i]->type == REDIRECT_OUT ||
					tokens[i]->type == APPEND || tokens[i]->type == HEREDOC)
		{
			if (tokens[i + 1] && tokens[i + 1]->type == WORD)
			{
				if (tokens[i]->type == REDIRECT_IN)
					cmd[cmd_count].input_redirect = strdup(tokens[i
							+ 1]->value);
				else if (tokens[i]->type == REDIRECT_OUT)
					cmd[cmd_count].output_redirect = strdup(tokens[i
							+ 1]->value);
				else if (tokens[i]->type == APPEND)
					cmd[cmd_count].append_redirect = strdup(tokens[i
							+ 1]->value);
				else if (tokens[i]->type == HEREDOC)
					cmd[cmd_count].heredoc_delimiter = strdup(tokens[i
							+ 1]->value);
				i++;
			}
			else
			{
				// fprintf(stderr, "Syntax error near unexpected token `%s`\n", tokens[i]->value);
				free(cmd);
				return (NULL);
			}
		}
		else
		{
			cmd[cmd_count].args = append_to_args(cmd[cmd_count].args,
					tokens[i]->value);
		}
		i++;
	}
	print_cmds(cmd);
	return (cmd);
}
