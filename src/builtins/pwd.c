/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/27 17:06:35 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
///

 int	builtin_pwd(t_cmd *cmd, t_data *data)
{
	char *buf;

	if (cmd->args[1] && ft_strchr(cmd->args[1], '-'))
	{
		write_error("minishell: pwd: -%c: invalid option\n", cmd->args[1][1]);
		return (data->exit_status = 1);
	}
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		ft_putstr_fd("pwd: error retrieving current directory: getcwd: ", 2);
		ft_putendl_fd("cannot access parent directories: No such file or directory",
			2);
		data->exit_status = 2;
		return (1);
	}
	return (ft_putendl_fd(buf, 1), free(buf), data->exit_status = 0);
}
