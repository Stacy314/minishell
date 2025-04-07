/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/07 21:29:52 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signal_flag = 0;

static void	free_main(t_data *data, t_token **tokens, t_cmd *cmd)
{
	if (data->input)
		free(data->input);
	if (tokens)
		free_tokens(tokens);
	if (cmd)
		free_cmd(cmd);
}

static int	check_args(int argc, char **argv)
{
	if (argv && argc > 1)
	{
		ft_putstr_fd("Minishell cannot accept arguments\n", STDOUT_FILENO);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	read_line_and_check(t_data *data)
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

static int	tokenize_and_parse(t_data *data)
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
		if (data->exit_status != 2)
			return (INPUT_ERROR);
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
		if (data.exit_status == 130)
			continue ;
		free_main(&data, data.tokens, data.cmd);
	}
	free_array(data.env);
	return (data.exit_status);
}
