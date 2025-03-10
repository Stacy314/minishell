/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/07 18:16:23 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// echo "exit_code ->$? user ->$USER home -> $HOME"(29)
// echo hi >./outfiles/outfile01 | echo bye (35)
// echo <123 <456 hi | echo 42 (37)
// echo "cat lol.c | cat > lol.c"(49)
// echo "cat lol.c '|' cat > lol.c"(51)
// echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER '' (75) - space in the end 
// echo '' "" (85)

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

void	builtin_echo(t_cmd *cmd, t_data *data)
{
	int	i;
	int	n_flag;


	i = 1;
	n_flag = 0;
	while (cmd->args[i] != NULL && is_option(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i] != NULL)
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	data->exit_status = 0;
	return ;
}
