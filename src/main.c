/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/05 21:30:46 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//apechkov@c3r7p6:~/projects/minishell$ cat << lim (ctrl + D)
//> 
//bash: warning: here-document at line 1 delimited by end-of-file (wanted `lim')

void	print_cmd_list(t_cmd *cmd_list, size_t count) 
{
	if (!cmd_list)
	{
		printf("print_cmd_list: cmd_list is NULL\n");
		return;
	}
	for (size_t i = 0; i < count; i++)
	{
		printf("=== Command %zu ===\n", i);
		if (!cmd_list[i].args)
		{
			printf("args: (none)\n");
			continue;
		}
		printf("args: ");
		for (size_t arg_index = 0; cmd_list[i].args[arg_index]; arg_index++)
			printf("\"%s\" ", cmd_list[i].args[arg_index]);
		printf("\n");

		printf("input_redirect:   %s\n",
			cmd_list[i].input_redirect ? cmd_list[i].input_redirect : "(none)");
		printf("output_redirect:  %s\n",
			cmd_list[i].output_redirect ? cmd_list[i].output_redirect : "(none)");
		printf("append_redirect:  %s\n",
			cmd_list[i].append_redirect ? cmd_list[i].append_redirect : "(none)");
		printf("heredoc_delimiter:%s\n",
			cmd_list[i].heredoc_delimiter ? cmd_list[i].heredoc_delimiter : "(none)");
		printf("\n");
	}
}


// Global flag for prompt control
// volatile sig_atomic_t g_prompt_flag = 0;

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;

	if (argv && argc > 1)
		return (ft_putstr_fd("Minishell cannot accept arguments\n",
				STDOUT_FILENO), EXIT_FAILURE);
	cmd = init_structure(&data, env);
	signal_handler();
	while (1)
	{
		///////////////
		// if (isatty(fileno(stdin)))
		//	input = readline("minishell$ ");
		// else
		//{
		//	char *line = get_next_line(fileno(stdin));
		//	input = ft_strtrim(line, "\n");
		//	free(line);
		//}
		///////////////
		input = readline("minishell$ ");
		if (!input)
			return (printf("exit\n"), data.exit_status);
		if (*input)
			add_history(input);
		if (!input || *input == '\0' || ft_str_only_spaces(input))
		{
			free(input);
			continue ;
		}
		tokens = split_to_tokens(input, &data);
		free(input);
		if (tokens == (t_token **)(-1)) //do we need it?
		{
			continue ;
		}
		if (!tokens) //do we need it?
		{
			// fprintf(stderr, "Error: Failed to tokenize input\n");
			continue ;
		}
		cmd = parse_tokens(tokens);
		if (!cmd)
		{
			free_tokens(tokens);
			// ft_putendl_fd("Error: Failed to parse tokens", STDERR_FILENO);
			continue ;
		}
		////////////////////////
		// print_cmd_list(cmd,4);
		execute(tokens, cmd, &data, env);
	}
	clear_history();
	return (data.exit_status);
}
