/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 18:39:00 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

// need to fix:

//"" " "(need to check)

// echo hi>>4 >>5 >>6 (should create be 3 append's redirects)

//&& and || with parenthesis for priorities (no segfault, maybe print error)

// what will be in cmd if there are few redirects?

//echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''
//echo '' ""

// fixed:
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

void debug_print_cmd(t_cmd *cmd)
{
    int i;

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
    printf("\n============================\n");
}

void parse_redirects(t_cmd *cmd, t_token *token, t_token_type type)
{
    char ***redirects;
    int count = 0;

    printf("⏳ parse_redirects: token->value = %s, token->type = %d\n", token->value, type);

    if (type == REDIRECT_IN)
        redirects = &cmd->input_redirects;
    else if (type == REDIRECT_OUT)
        redirects = &cmd->output_redirects;
    else if (type == APPEND)
        redirects = &cmd->append_redirects;
    else
    {
        printf("❌ Unknown token type!\n");
        return;
    }

    // Проверяем, есть ли уже массив редиректов
    if (!*redirects)
    {
        printf("🔍 First redirect detected, allocating memory\n");
        *redirects = ft_calloc(2, sizeof(char *));
        if (!*redirects)
        {
            perror("ft_calloc");
            return;
        }
        (*redirects)[0] = ft_strdup(token->value);
        (*redirects)[1] = NULL;
        printf("✅ Added redirect: %s\n", (*redirects)[0]);
        return;
    }

    // Если массив уже есть, считаем количество элементов
    while ((*redirects)[count])
        count++;

    printf("🔄 Expanding redirect array (current size: %d)\n", count);

    // Расширяем массив
    char **new_redirects = realloc(*redirects, (count + 2) * sizeof(char *));
    if (!new_redirects)
    {
        perror("realloc");
        return;
    }

    *redirects = new_redirects;
    (*redirects)[count] = ft_strdup(token->value);
    (*redirects)[count + 1] = NULL;
    printf("✅ Added redirect: %s (new size: %d)\n", (*redirects)[count], count + 1);
}



// char **add_to_redirect_list(char **list, char *new_redirect)
// {
//     int i = 0;
//     while (list && list[i])
//         i++;

//     char **new_list = malloc(sizeof(char *) * (i + 2));
//     if (!new_list)
//     {
//         perror("malloc");
//         return NULL;
//     }

//     i = 0;
//     while (list && list[i])
//     {
//         new_list[i] = list[i];
//         i++;
//     }

//     new_list[i] = strdup(new_redirect);
//     new_list[i + 1] = NULL;
//     free(list);
//     return new_list;
// }

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
		// free(args);
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

// t_cmd	*parse_tokens(t_token **tokens)
//{
//	t_cmd	*cmd;
//	int		cmd_count;
//	int		i;
//	int		last_index;

//	cmd = malloc(sizeof(t_cmd) * MAX_COMMANDS);
//	if (!cmd)
//	{
//		perror("malloc");
//		//free(tokens);
//		return (NULL);
//	}
//	cmd_count = 0;
//	i = 0;
//	cmd[cmd_count] = (t_cmd){NULL, NULL, NULL, NULL, NULL, NULL, NULL};
//	last_index = 0;
//	while (tokens[last_index])
//		last_index++;
//	if (tokens[0]->type == PIPE || tokens[last_index - 1]->type == PIPE)
		//$notexists | ls | $notexists
//	{
//		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
//		free(cmd);
//		return (NULL);
//	}
//	while (tokens[i])
//	{
//		if (tokens[i]->type == PIPE)
//		{
//			cmd_count++;
//			cmd[cmd_count] = (t_cmd){NULL, NULL, NULL, NULL, NULL, NULL, NULL};
//		}
//		else if (tokens[i]->type == REDIRECT_IN
//				|| tokens[i]->type == REDIRECT_OUT ||
//					tokens[i]->type == APPEND || tokens[i]->type == HEREDOC)
//		{
//			if (tokens[i + 1] && tokens[i + 1]->type == WORD)
//			{
//				if (tokens[i]->type == REDIRECT_IN)
//					cmd[cmd_count].input_redirect = strdup(tokens[i
//							+ 1]->value);
//				else if (tokens[i]->type == REDIRECT_OUT)
//					cmd[cmd_count].output_redirect = strdup(tokens[i
//							+ 1]->value);
//				else if (tokens[i]->type == APPEND)
//					cmd[cmd_count].append_redirect = strdup(tokens[i
//							+ 1]->value);
//				else if (tokens[i]->type == HEREDOC)
//					cmd[cmd_count].heredoc_delimiter = strdup(tokens[i
//							+ 1]->value);
//				i++;
//			}
//			else
//			{
//				// fprintf(stderr, "Syntax error near unexpected token `%s`\n",
					//tokens[i]->value);
//				free(cmd);
//				return (NULL);
//			}
//		}
//		else
//		{
//			cmd[cmd_count].args = append_to_args(cmd[cmd_count].args,
//					tokens[i]->value);
//		}
//		i++;
//	}
//	//print_cmds(cmd);
//	return (cmd);
//}

t_cmd	*parse_tokens(t_token **tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*prev;
	int		i;
	t_cmd	*new_cmd;

	head = NULL;
	current = NULL;
	prev = NULL;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == PIPE && (!tokens[i + 1] || i == 0))
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
			return (NULL);
		}
		new_cmd = ft_calloc(1, sizeof(t_cmd));
		if (!new_cmd)
			return (perror("malloc"), NULL);
		new_cmd->args = NULL;
		new_cmd->input_redirects = NULL;
		new_cmd->output_redirects = NULL;
		new_cmd->append_redirects = NULL;
		new_cmd->heredoc_delimiter = NULL;
		new_cmd->next = NULL;
		if (!head)
			head = new_cmd;
		else
			prev->next = new_cmd;
		current = new_cmd;
		prev = new_cmd;
		while (tokens[i] && tokens[i]->type != PIPE)
		{
			if (tokens[i]->type == REDIRECT_IN || tokens[i]->type == REDIRECT_OUT
				|| tokens[i]->type == APPEND || tokens[i]->type == HEREDOC)
			{
				if (!tokens[i + 1] || tokens[i + 1]->type != WORD)
				{
					ft_putendl_fd("minishell: syntax error near unexpected token", 2);
					return (NULL);
				}
				parse_redirects(current, tokens[i + 1], tokens[i]->type);
				i++;
			}
			else
			{
				current->args = append_to_args(current->args, tokens[i]->value);
			}
			i++;
		}
		if (tokens[i] && tokens[i]->type == PIPE)
			i++;
	}
	return (head);
}
