/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/31 21:54:22 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NEED TO FIX:
// expot a
//_ value
// pwd (don't call getcwd every time)
// signals

// interactive/ non-int.

// Wildcards * (print error)

// #invalid command, followed by empty variable, should clear the exit code
// doesntexist
// $EMPTY
// echo $?

// # Local variables are options
// aa==vv
// echo $aa

volatile sig_atomic_t	g_signal_flag = 0;

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;

	input = NULL;
	tokens = NULL;
	cmd = NULL;
	if (argv && argc > 1)
		return (ft_putstr_fd("Minishell cannot accept arguments\n",
				STDOUT_FILENO), EXIT_FAILURE);
	if (!init_data(&data, env))
		return (EXIT_FAILURE);
	set_signals_main();
	while (1)
	{
		if (g_signal_flag == SIGINT) //move to sig
		{
			data.exit_status = 130;
			g_signal_flag = 0;
		}
		input = readline("minishell$ ");
		if (!input)
			return (printf("exit\n"), free_array(data.env), /* free_all(&data, tokens,cmd), */ data.exit_status);
		if (*input == '\0' || ft_str_only_spaces(input))
		{
			//free_all(&data, tokens,cmd);
			continue ;
		}
		if (*input)
			add_history(input);
		data.input = input;
		tokens = split_to_tokens(input, &data);
		if (!tokens)
		{
			free_all(&data, tokens,cmd);
			continue; //need to change
			//return (EXIT_FAILURE); //for memory fail
		}
		cmd = parse_tokens(tokens, &data);
		if (!cmd)
		{
			
			free_all(&data, tokens,cmd);
			continue; //need to change
			//return (EXIT_FAILURE); //for memory fail
		}
		execute(tokens, cmd, &data); 
		free_all(&data, tokens,cmd);
	}
	free_array(data.env);
	// clear_history();
	return (data.exit_status);
}
