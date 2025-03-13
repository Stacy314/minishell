/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/13 22:48:59 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Global flag for prompt control
// volatile sig_atomic_t g_prompt_flag = 0;

//Wildcards * (print error)

// #invalid command, followed by empty variable, should clear the exit code
// doesntexist
// $EMPTY
// echo $?

// # Local variables are options
// aa==vv
// echo $aa

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
		//free(input);
		if (tokens == (t_token **)(-1)) //do we need it?
		{
			continue ;
		}
		if (!tokens) //do we need it?
		{
			// fprintf(stderr, "Error: Failed to tokenize input\n");
			continue ;
		}
		cmd = parse_tokens(tokens, &data);
		if (!cmd)
		{
			free_tokens(tokens);
			// ft_putendl_fd("Error: Failed to parse tokens", STDERR_FILENO);
			continue ;
		}
		execute(tokens, cmd, &data, env, input);
		free(input);
	}
	clear_history();
	return (data.exit_status);
}
