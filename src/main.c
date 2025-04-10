/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 22:07:35 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

sig_atomic_t	g_signal_flag = 0;

static int	check_args(int argc, char **argv)
{
	if (argv && argc > 1)
	{
		ft_putstr_fd("Minishell cannot accept arguments\n", STDOUT_FILENO);
		return (ERROR);
	}
	return (SUCCESS);
}

int	read_line_and_check(t_data *data)
{
	data->input = readline("minishell$ ");
	if (!data->input)
	{
		printf("exit\n");
		free_array(data->env);
		return (INPUT_STOP);
	}
	if (*data->input == '\0' || ft_str_only_spaces(data->input))
		return (INPUT_CONTINUE);
	if (g_signal_flag == SIGINT)
	{
		data->exit_status = 130;
		g_signal_flag = 0;
	}
	add_history(data->input);
	return (INPUT_SUCCESS);
}

int	tokenize_and_parse(t_data *data)
{
	data->tokens = split_to_tokens(data->input, data);
	if (!data->tokens)
	{
		free(data->input);
		if (data->exit_status != 2)
			return (INPUT_ERROR);
		return (INPUT_CONTINUE);
	}
	data->cmd = parse_tokens(data->tokens, data);
	if (!data->cmd)
	{
		free_tokens(data->tokens);
		free(data->input);
		return (INPUT_CONTINUE);
	}
	return (INPUT_SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		ret;

	if (check_args(argc, argv) == ERROR)
		return (EXIT_FAILURE);
	if (!init_data(&data, env))
		return (EXIT_FAILURE);
	set_signals_main();
	while (1)
	{
		ret = prepare_input(&data);
		if (ret == INPUT_STOP)
			return (data.exit_status);
		if (ret == INPUT_CONTINUE)
			continue ;
		if (ret == INPUT_ERROR)
			return (EXIT_FAILURE);
		execute(data.tokens, data.cmd, &data);
		free_main(&data, data.tokens, data.cmd);
	}
	free_array(data.env);
	return (data.exit_status);
}
