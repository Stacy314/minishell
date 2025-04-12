/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 21:31:41 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//(ulimit -v 130000; valgrind --leak-check=full ./minishell)

sig_atomic_t	g_signal_flag = 0;

int	main_loop(t_data *data, char *last_value)
{
	while (1)
	{
		data->input = readline("minishell$ ");
		if (!data->input)
			return (printf("exit\n"), free_array(data->env), data->exit_status);
		if (*data->input == '\0' || ft_str_only_spaces(data->input))
			continue ;
		if (g_signal_flag == SIGINT)
		{
			data->exit_status = 130;
			g_signal_flag = 0;
		}
		add_history(data->input);
		data->tokens = split_to_tokens(data->input, data);
		if (!data->tokens)
		{
			free(data->input);
			if (data->exit_status != 2)
				return (EXIT_FAILURE);
			continue ;
		}
		data->cmd = parse_tokens(data->tokens, data);
		if (!data->cmd)
		{
			(free(data->input), free_tokens(data->tokens));
			continue ;
		}
		last_value = find_last_value(data->tokens);
		if (last_value)
			update_underscore(data, last_value);
		(execute(data->tokens, data->cmd, data), free(data->input),
			free_tokens(data->tokens), free_cmd(data->cmd));
	}
}

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
	main_loop(&data, last_value);
	return (data.exit_status);
}
