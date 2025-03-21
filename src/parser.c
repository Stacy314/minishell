/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/21 22:08:06 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// need to fix:

//"" (: command not found, EC - 127)
// echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER '' (space in the end) 
// echo '' "" (print space)
// exit "" (bash: exit: : numeric argument required, EC - 2)

// cat <minishell.h|ls

// export a="s -lsa"
//		l$a

// echo hi | echo hi | (should open input)

//$notexists | ls | $notexists

//> $notexists echo "haha" (bash: $notexists: ambiguous redirect)

//export A="haha         123"
//echo $A  (should print haha 123 with one space)

//export A='"echo hi"'
//$A  ("echo: command not found)

// void	debug_print_cmd(t_cmd *cmd)
// {
// 	int	i;

void	debug_print_cmd(t_cmd *cmd)
{
	int	i;
	int	cmd_num = 1;

	printf("\n==== DEBUG CMD STRUCTURE ====\n");
	while (cmd)
	{
		printf("Command #%d\n", cmd_num++);

		// Args
		printf("Args: ");
		if (cmd->args)
		{
			for (i = 0; cmd->args[i]; i++)
				printf("\"%s\" ", cmd->args[i]);
		}
		else
			printf("(null)");
		printf("\n");

		// Input Redirects
		printf("Input Redirects: ");
		if (cmd->input_redirects)
		{
			for (i = 0; cmd->input_redirects[i]; i++)
				printf("\"%s\" ", cmd->input_redirects[i]);
		}
		else
			printf("(null)");
		printf("\n");

		// Output Redirects
		printf("Output Redirects: ");
		if (cmd->output_redirects)
		{
			for (i = 0; cmd->output_redirects[i]; i++)
				printf("\"%s\" ", cmd->output_redirects[i]);
		}
		else
			printf("(null)");
		printf("\n");

		// Append Redirects
		printf("Append Redirects: ");
		if (cmd->append_redirects)
		{
			for (i = 0; cmd->append_redirects[i]; i++)
				printf("\"%s\" ", cmd->append_redirects[i]);
		}
		else
			printf("(null)");
		printf("\n");

		// Heredoc Delimiters
		printf("Heredoc Delimiters: ");
		if (cmd->heredoc_delimiter)
		{
			for (i = 0; cmd->heredoc_delimiter[i]; i++)
				printf("\"%s\" ", cmd->heredoc_delimiter[i]);
		}
		else
			printf("(null)");
		printf("\n");

		printf("============================\n");
		cmd = cmd->next;
	}
}


static int	parse_redirects(t_cmd *cmd, t_token *token, t_token_type type)
{
	char	***redirects;
	int		count;
	char	**new_redirects;
	//(void)token;

	count = 0;
	if (type == REDIRECT_IN)
		redirects = &cmd->input_redirects;
	else if (type == REDIRECT_OUT)
		redirects = &cmd->output_redirects;
	else if (type == APPEND)
		redirects = &cmd->append_redirects;
	else if (type == HEREDOC)
		redirects = &cmd->heredoc_delimiter;
	else
	{
		return (SUCCESS);
	}
	if (!*redirects)
	{
		*redirects = ft_calloc(2, sizeof(char *));
		if (!*redirects)
		{
			perror("ft_calloc");
			return (ERROR);
		}
		(*redirects)[0] = ft_strdup(token->value);
		(*redirects)[1] = NULL;
		//return (ERROR);
	}
	while ((*redirects)[count])
		count++;
	new_redirects = realloc(*redirects, (count + 2) * sizeof(char *)); //forbidden func
	if (!new_redirects)
	{
		perror("realloc");
		return (ERROR);
	}
	*redirects = new_redirects;
	(*redirects)[count] = ft_strdup(token->value);
	if (!(*redirects)[count])
	{
		perror("ft_strdup");
		return (ERROR);
	}
	(*redirects)[count + 1] = NULL;
	return (SUCCESS);
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
	{
		perror("calloc");
		// free(args);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[len] = strdup(new_arg); //
	new_args[len + 1] = NULL;
	free(args);
	return (new_args);
}

t_cmd	*parse_tokens(t_token **tokens, t_data *data)
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
		if (tokens[i]->type == LOGICAL_AND || tokens[i]->type == LOGICAL_OR)
		{
			write_error("minishell: syntax error near unexpected token `%s'\n",
				tokens[i]->value);
			data->exit_status = 2;
			return (NULL);
		}
		if (tokens[i]->type == PIPE && (!tokens[i + 1] || i == 0))
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'",
				2);
			data->exit_status = 2;
			return (NULL);
		}
		new_cmd = ft_calloc(1, sizeof(t_cmd));
		if (!new_cmd)
			return (perror("calloc"), NULL);
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
			if (tokens[i]->type == REDIRECT_IN
				|| tokens[i]->type == REDIRECT_OUT || tokens[i]->type == APPEND
				|| tokens[i]->type == HEREDOC)
			{
				if (!tokens[i + 1] || tokens[i + 1]->type != WORD)
				{
					write_error("minishell: syntax error near unexpected token `%s'\n",
						tokens[i + 1] ? tokens[i + 1]->value : "newline");
					// write_error("minishell: %s: ambiguous redirect", tokens[i
						//+ 1]->value); //> $lol (segfault)
					// ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
						//2);
					data->exit_status = 2;
					free (new_cmd);
					return (NULL);
				}
				if (!parse_redirects(current, tokens[i + 1], tokens[i]->type))
				{
					//print_error
					free (new_cmd);
					return (NULL);
				}
				i++;
			}
			else
			{
				current->args = append_to_args(current->args, tokens[i]->value);
				if (!current->args)
				{
					//print_error
					free (new_cmd);
					return (NULL);
				}
			}
			i++;
		}
		if (tokens[i] && tokens[i]->type == PIPE)
			i++;
	}
	// debug_print_cmd(head);
	//free (new_cmd);
	return (head);
}
