/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/21 23:27:38 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//NEED TO FIX:
//expot a
//_ value
//pwd (don't call getcwd every time)
//signals

//interactive/ non-int.

// Wildcards * (print error)

// #invalid command, followed by empty variable, should clear the exit code
// doesntexist
// $EMPTY
// echo $?

// # Local variables are options
// aa==vv
// echo $aa

volatile sig_atomic_t g_signal_flag = 0;

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;

	if (argv && argc > 1)
		return (ft_putstr_fd("Minishell cannot accept arguments\n",
				STDOUT_FILENO), EXIT_FAILURE);
	if (!init_data(&data, env))
		return (EXIT_FAILURE);
	set_signals_main();
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
			return (printf("exit\n")/*, free_env(data.env)*/, data.exit_status);
		if (*input == '\0' || ft_str_only_spaces(input)) //need to check
		{
			free(input);
			continue ;
		}
		// if (g_prompt_flag == 1)
        // {
        //     data.exit_status = 130;
        //     g_prompt_flag = 0;
        // }
		// if (g_prompt_flag == 2)
        // {
        //     data.exit_status = 131;
        //     g_prompt_flag = 0;
        // }
		data.input = input;
		if (*input)
			add_history(input);
		tokens = split_to_tokens(input, &data);
		if (tokens == (t_token **)(-1) || !tokens)
		{
			free(input);
			//free_tokens(tokens);
			continue ;
		}
		cmd = parse_tokens(tokens, &data);
		if (!cmd)
		{
			free_tokens(tokens);
			free(cmd);
			free(input);
			continue ;
		}
		execute(tokens, cmd, &data, env);
		free(input);
		free_tokens(tokens);
		free_cmd(cmd);
	}
	//free_env(data.env);
	clear_history();
	return (data.exit_status);
}
