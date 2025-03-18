/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/18 14:13:48 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Global flag for prompt control
// volatile sig_atomic_t g_prompt_flag = 0;

// Wildcards * (print error)

// #invalid command, followed by empty variable, should clear the exit code
// doesntexist
// $EMPTY
// echo $?

// # Local variables are options
// aa==vv
// echo $aa

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

pid_t	g_main_pid;

void	init_main_pid(void)
{
	g_main_pid = getpid();
}

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
	init_main_pid();
	signal_handler();
	while (1)
	{
		// /////////////
		// if (isatty(fileno(stdin)))
		// 	input = readline("minishell$ ");
		// else
		// {
		// 	char *line = get_next_line(fileno(stdin));
		// 	input = ft_strtrim(line, "\n");
		// 	free(line);
		// }
		// /////////////
		input = readline("minishell$ ");
		if (!input)
			return (printf("exit\n"), free_env(data.env), data.exit_status);
		if (!input || *input == '\0' || ft_str_only_spaces(input))
		{
			free(input);
			continue ;
		}
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
	free_env(data.env);
	clear_history();
	return (data.exit_status);
}
