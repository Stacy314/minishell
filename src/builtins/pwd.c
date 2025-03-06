/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 16:55:34 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_pwd(t_cmd *cmd, t_data *data)
{
	char	*buf;

	if (cmd->args[1] && ft_strchr(cmd->args[1], '-'))
	{
		printf("bash: pwd: -%c: invalid option\n", cmd->args[1][1]);
		return (data->exit_status = 1);
	}
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		ft_putendl_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories\n",
			data->exit_status = 2);
	return (ft_putendl_fd(buf, 1), free(buf), data->exit_status = 0);
}
