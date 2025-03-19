/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/19 16:27:16 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// need to fix:

//"" (: command not found, EC - 127)

// export a="s -lsa"
//		l$a

// echo hi | echo hi | (should open input or error)

// cat <minishell.h|ls

// cat <<HEREDOC (we dont have HEREDOC)

// exit "" (bash: exit: : numeric argument required, EC - 2)

//&& and || with parenthesis for priorities (no segfault, maybe print error)

// fixed:

// echo '' ""

// awk 'BEGIN{for(i=1;i<=10;i++){for(j=1;j<=10;j++){printf("%4d ",i*j)} printf("\n")}}'
	///dev/null | tail -n 10
// Expected tkens:
// 1) "awk"
// 2) "'BEGIN{for(i=1;i<=10;i++){for(j=1;j<=10;j'){printf("%4d ",i*j)} printf("\n")}}'"
// 3) "/dev/null"
// 4) "|"
// 5) "tail"
// 6) "-n"
// 7) "10"

// echo hi>>4 >>5 >>6 (should create be 3 append's redirects)
// echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER '' (space in the end)
// echo "cat lol.c '|' cat > lol.c" (should print cat lol.c '|' cat > lol.c)
// grep hi "<infile" <         ./test_files/infile ("<infile"  - one arg,
	//it isnt redir)
// echo hi | echo hi | (error with pipe, in our minishell - execve: No such file
// or directory)
//| echo hi (bash: syntax error near unexpected token `|', in our minishell -
// execve: No such file or directory + Conditional jump)
// empty line (segfault), should print prompt again
//				(segfault) - tab
//                (segfault) - space
//"" (should be empty arg)
// echo "" (should print a new line)
// echo $SHLVL (parse of env, also with "" and '')
// echo "" "        h           a           " (should print         h           a           )
//"echo" hi (should print hi)
//"ls" and 'ls' (should work as regular ls)
// e"ch"o hi (should print hi)
//'echo' hi (should print hi)
// echo hi '$USER is great, home is $HOME' (hi $USER is great, home is $HOME)
// echo hi "$USER is great, home is $HOME" (hi apechkov is great, home is
/// home/apechkov)
//"<" sasdad
//<: command not found (in bash), in minishell - it is a command
//"echo hi" and 'echo hi' (should pars as one arg, echo hi: command not found)
// echo "|" echo (should print - | echo)
// echo " | "(should print -  | )

void	debug_print_cmd(t_cmd *cmd)
{
	int	i;

	printf("\n==== DEBUG CMD STRUCTURE ====\n");
	printf("Args: ");
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
	printf("Input Redirects: ");
	if (cmd->input_redirects)
	{
		i = 0;
		while (cmd->input_redirects[i])
		{
			printf("\"%s\" ", cmd->input_redirects[i]);
			i++;
		}
	}
	else
		printf("(null)");
	printf("\n");
	printf("Output Redirects: ");
	if (cmd->output_redirects)
	{
		i = 0;
		while (cmd->output_redirects[i])
		{
			printf("\"%s\" ", cmd->output_redirects[i]);
			i++;
		}
	}
	else
		printf("(null)");
	printf("\n");
	printf("Append Redirects: ");
	if (cmd->append_redirects)
	{
		i = 0;
		while (cmd->append_redirects[i])
		{
			printf("\"%s\" ", cmd->append_redirects[i]);
			i++;
		}
	}
	else
		printf("(null)");
	printf("\n");
	printf("Heredoc: ");
	if (cmd->heredoc_delimiter)
	{
		int i = 0;
		while (cmd->heredoc_delimiter[i])
		{
			printf("%s", cmd->heredoc_delimiter[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("(null)");
	printf("\n============================\n");
}

int check_initial_syntax_errors(t_token **tokens, t_data *data)
{
	int i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == LOGICAL_AND || tokens[i]->type == LOGICAL_OR)
		{
			write_error("minishell: syntax error near unexpected token `%s'\n", tokens[i]->value);
			data->exit_status = 2;
			return 1;
		}
		if (tokens[i]->type == PIPE && (i == 0 || !tokens[i + 1]))
		{
			write_error("minishell: syntax error near unexpected token `|'\n");
			data->exit_status = 2;
			return 1;
		}
		i++;
	}
	return 0;
}

int parse_redirects(t_cmd *cmd, t_token *token, t_token_type type)
{
	char ***redirects = get_redirect_target(cmd, type);
	if (!redirects)
		return (SUCCESS);
	if (!*redirects)
		return initialize_redirect_array(redirects, token->value);
	else
		return append_redirect_value(redirects, token->value);
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
	new_args = ft_calloc(sizeof(char *) * (len + 2), 1); //
	if (!new_args)
		return (perror("calloc"), NULL);
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

t_cmd	*parse_tokens(t_token **tokens, t_data *data)
{
	t_cmd *head = NULL;
	int i = 0;

	if (check_initial_syntax_errors(tokens, data))
		return NULL;
	if (!build_command_list(&head, tokens, data, &i))
	{
		free_cmd(head);
		return NULL;
	}
	return head;
}
