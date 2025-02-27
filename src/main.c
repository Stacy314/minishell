/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/27 19:54:33 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_cmd_list(t_cmd *cmd_list, size_t count) // need to del
{
	for (size_t i = 0; i < count; i++)
	{
		printf("=== Command %zu ===\n", i);
		printf("args: ");
		if (cmd_list[i].args)
		{
			size_t arg_index = 0;
			while (cmd_list[i].args[arg_index])
			{
				printf("\"%s\" ", cmd_list[i].args[arg_index]);
				arg_index++;
			}
		}
		else
		{
			printf("(none)");
		}
		printf("\n");
		printf("input_redirect:   %s\n",
				cmd_list[i].input_redirect ? cmd_list[i].input_redirect : "(none)");
		printf("output_redirect:  %s\n",
				cmd_list[i].output_redirect ? cmd_list[i].output_redirect : "(none)");
		printf("append_redirect:  %s\n",
				cmd_list[i].append_redirect ? cmd_list[i].append_redirect : "(none)");
		printf("heredoc_delimiter:%s\n",
				cmd_list[i].heredoc_delimiter ? cmd_list[i].heredoc_delimiter : "(none)");
		if (cmd_list[i].data)
		{
			printf("data->some_value: %d\n", cmd_list[i].data->some_value);
		}
		else
		{
			printf("data:            (null)\n");
		}
		printf("\n");
	}
}

// Global flag for prompt control
//volatile sig_atomic_t g_prompt_flag = 0;

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;

	if (argv && argc > 1)
		return (ft_putstr_fd("Minishell cannot accept arguments\n",
								STDOUT_FILENO),
				EXIT_FAILURE);
	cmd = init_structure(&data, env);
	signal_handler();
	while (1)
	{
		///////////////
		//if (isatty(fileno(stdin)))
		//	input = readline("minishell$ ");
		//else
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
            continue;
        }

        tokens = split_to_tokens(input, &data);
        free(input);

        if (tokens == (t_token **)(-1))
        {
            continue;
        }

        if (!tokens)
        {
            //fprintf(stderr, "Error: Failed to tokenize input\n");
            continue;
        }

        cmd = parse_tokens(tokens);
        if (!cmd)

		{
			free_tokens(tokens);
            //ft_putendl_fd("Error: Failed to parse tokens", STDERR_FILENO);
           return(0);
        }

		////////////////////////
		//print_cmd_list(cmd,4);
		execute(tokens, cmd, &data, env);
	}
	clear_history();
	return (data.exit_status);
}
