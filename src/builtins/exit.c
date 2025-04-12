/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/12 19:53:31 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_error(const char *msg)
{
	if (isatty(0) || isatty(1))
		printf("exit\n");
	write_error(msg);
}

int	builtin_exit(t_cmd *cmd, t_data *data, t_token **tokens, int token_index)
{
	long	exit_code;
	int		error;

	error = 0;
	if (!cmd->args[token_index + 1])
	{
		if (!data->is_child)
			printf("exit\n");
		return ((free_all(data, tokens, cmd), close_fd_main(data),
				exit(data->exit_status), 1));
	}
	exit_code = ft_atol(cmd->args[1], &error);
	if (error)
	{
		if (isatty(0) || isatty(1))
			printf("exit\n");
		(write_error("minishell: exit: %s: numeric argument required\n",
				cmd->args[1]), free_all(data, tokens, cmd), close_fd_main(data),
			exit(2));
	}
	if (cmd->args[token_index + 2])
		return (print_error("minishell: exit: too many arguments\n"),
			data->exit_status = 1, 1);
	return (printf("exit\n"), free_all(data, tokens, cmd), close_fd_main(data),
		exit(exit_code % 256), 1);
}
