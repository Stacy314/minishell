/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/02 21:23:44 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// NEED TO FIX:
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
	t_data	data;
	char	*last_value;

	last_value = NULL;
	if (argv && argc > 1)
		return (ft_putstr_fd("Minishell cannot accept arguments\n",
				STDOUT_FILENO), EXIT_FAILURE);
	if (!init_data(&data, env))
		return (EXIT_FAILURE);
	set_signals_main();
	while (1)
	{
		if (g_signal_flag == SIGINT) // move to sig
		{
			data.exit_status = 130;
			g_signal_flag = 0;
		}
		data.input = readline("minishell$ ");
		if (!data.input)
			return (printf("exit\n"), free_array(data.env), data.exit_status);
		if (*data.input == '\0' || ft_str_only_spaces(data.input))
			continue ;
		if (*data.input)
			add_history(data.input);
		data.tokens = split_to_tokens(data.input, &data);
		if (!data.tokens)
		{
			free(data.input);
			if (data.exit_status != 2)
				return (EXIT_FAILURE);
			continue ;
		}
		data.cmd = parse_tokens(data.tokens, &data);
		if (!data.cmd)
		{
			free_tokens(data.tokens);
			free(data.input);
			if (data.exit_status != 2)
				return (EXIT_FAILURE);
			continue ;
		}
		execute(data.tokens, data.cmd, &data);
		last_value = find_last_value(data.tokens);
		update_underscore(&data, last_value); // update only success exec
		free_main(&data, data.tokens, data.cmd);
	}
	free_array(data.env);
	// clear_history();
	return (data.exit_status);
}
