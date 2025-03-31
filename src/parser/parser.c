/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/31 15:40:42 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// need to fix:
// cat << $USER (wrong expansion)
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

// export A="haha         123"
// echo $A  (should print haha 123 with one space)

// export A='"echo hi"'
//$A  ("echo: command not found)

// void	debug_print_cmd(t_cmd *cmd)
// {
// 	int	i;

//////////////////////////////////////////////////////////////////////
void	debug_print_cmd(t_cmd *cmd)
{
	int	i;
	int	cmd_num;

	cmd_num = 1;
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
//////////////////////////////////////////////////////////////////////

int	check_initial_syntax_errors(t_token **tokens, t_data *data)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == LOGICAL_AND || tokens[i]->type == LOGICAL_OR)
		{
			write_error("minishell: syntax error near unexpected token `%s'\n",
				tokens[i]->value);
			data->exit_status = 2;
			return (1);
		}
		if (tokens[i]->type == PIPE && (i == 0 || !tokens[i + 1]))
		{
			write_error("minishell: syntax error near unexpected token `|'\n");
			data->exit_status = 2;
			return (1);
		}
		i++;
	}
	return (0);
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
	new_args[len] = ft_strdup(new_arg);
	new_args[len + 1] = NULL;
	free(args);
	return (new_args);
}

int	fill_cmd(t_cmd *cmd, t_token **tokens, t_data *data, int *i)
{
	while (tokens[*i] && tokens[*i]->type != PIPE)
	{
		if (tokens[*i]->type == REDIRECT_IN || tokens[*i]->type == REDIRECT_OUT
		|| tokens[*i]->type == APPEND || tokens[*i]->type == HEREDOC)
		{
			if (!handle_redirect(cmd, tokens, data, i))
				return (0);
		}
		else
		{
			cmd->args = append_to_args(cmd->args, tokens[*i]->value);
			if (!cmd->args)
				return (0);
		}
		(*i)++;
	}
	return (1);
}

int	build_command_list(t_cmd **head, t_token **tokens, t_data *data, int *i)
{
	t_cmd	*prev;
	t_cmd	*current;

	prev = NULL;
	current = NULL;
	while (tokens[*i])
	{
		current = init_new_cmd();
		if (!current)
			return (0);
		if (!*head)
			*head = current;
		else
			prev->next = current;
		if (!fill_cmd(current, tokens, data, i))
			return (0);
		prev = current;
		if (tokens[*i] && tokens[*i]->type == PIPE)
			(*i)++;
	}
	return (1);
}

t_cmd	*parse_tokens(t_token **tokens, t_data *data)
{
	t_cmd	*head;
	int		i;

	head = NULL;
	i = 0;
	if (check_initial_syntax_errors(tokens, data))
		return (NULL);
	if (!build_command_list(&head, tokens, data, &i))
	{
		free_cmd(head);
		return (NULL);
	}
	//debug_print_cmd(head);
	return (head);
}
