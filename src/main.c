/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/27 22:27:56 by apechkov         ###   ########.fr       */
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

	if (argv && argc > 1)
		return (ft_putstr_fd("Minishell cannot accept arguments\n",
				STDOUT_FILENO), EXIT_FAILURE);
	if (!init_data(&data, env))
		return (EXIT_FAILURE);
	set_signals_main();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			return (printf("exit\n"), free_array(data.env), data.exit_status);
				// need free all struct
		if (*input == '\0' || ft_str_only_spaces(input))                      
			// need to check
		{
			free(input);
			free_array(data.env);
			continue ;
		}
		if (g_signal_flag == SIGINT)
		{
			data.exit_status = 130;
			g_signal_flag = 0;
		}
		data.input = input;
		if (*input)
			add_history(input);
		tokens = split_to_tokens(input, &data);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		cmd = parse_tokens(tokens, &data);
		if (!cmd)
		{
			free_tokens(tokens);
			free(input);
			continue ;
		}
		execute(tokens, cmd, &data);
		free(input);
		free_tokens(tokens);
		free_cmd(cmd);
	}
	free_array(data.env);
	// clear_history();
	return (data.exit_status);
}
