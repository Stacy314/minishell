/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/01 15:38:53 by apechkov         ###   ########.fr       */
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
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;
	//char	*last_value;

	//last_value = NULL;
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
		tokens = split_to_tokens(data.input, &data);
		if (!tokens)
		{
			free(data.input);
			if (data.exit_status != 2)
				return (EXIT_FAILURE);
			continue ;
		}
		cmd = parse_tokens(tokens, &data);
		if (!cmd)
		{
			free_all(&data, tokens, cmd); //del cmd
			if (data.exit_status != 2)
				return (EXIT_FAILURE);
			continue ;
		}
		execute(tokens, cmd, &data);
		//last_value = find_last_value(tokens);
		//update_underscore(&data, last_value); // update only success exec
		free_all(&data, tokens, cmd);
	}
	free_array(data.env);
	// free(data.env);
	// clear_history();
	return (data.exit_status);
}
