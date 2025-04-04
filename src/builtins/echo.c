/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/03 22:01:22 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER '' (75)
	//- space in the end
// echo '' "" (85)

// echo $"" (should print \n) (go crazy)

int	is_option(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	if (arg[1] == '\0')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

void	builtin_echo(t_cmd *cmd, t_data *data, int token_index)
{

	int	n_flag;

	token_index++;
	n_flag = 0;
	while (cmd->args[token_index] != NULL && is_option(cmd->args[token_index]))
	{
		n_flag = 1;
		token_index++;
	}
	while (cmd->args[token_index] != NULL)
	{
		ft_putstr_fd(cmd->args[token_index], STDOUT_FILENO);
		if (cmd->args[token_index + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		token_index++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	data->exit_status = 0;
	return ;
}
