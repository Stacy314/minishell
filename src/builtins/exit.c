/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/06 19:51:57 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// exit "" (bash: exit: : numeric argument required, EC - 2)

long	ft_atol(const char *str/*, int *error*/)
{
	unsigned long	result;
	int				sign;
	int				i;

	result = 0;
	sign = 1;
	i = 0;
	while (isspace((unsigned char)str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (-1);
	while (str[i] && isdigit((unsigned char)str[i]))
	{
		if (sign == 1)
		{
			if (result > (unsigned long)LONG_MAX / 10
				|| (result == (unsigned long)LONG_MAX / 10 && (str[i]
						- '0') > (int)((unsigned long)LONG_MAX % 10)))
				return (-1);
		}
		else
		{
			if (result > ((unsigned long)LONG_MAX + 1) / 10
				|| (result == ((unsigned long)LONG_MAX + 1) / 10 && (str[i]
						- '0') > (int)(((unsigned long)LONG_MAX + 1) % 10)))
				return (-1);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	while (isspace((unsigned char)str[i]))
		i++;
	if (str[i] != '\0')
		return (-1);
	if (sign == -1 && result == (unsigned long)LONG_MAX + 1)
		return (LONG_MIN);
	return ((long)(result * sign));
}


int	builtin_exit(t_cmd *cmd, t_data *data)
{
	long	exit_code;
	//int		error;

	//error = 0;
	printf("exit\n");
	if (!cmd->args[1])
		exit(data->exit_status);
	exit_code = ft_atol(cmd->args[1]/*, &error*/);
	if (exit_code == -1)
	{
		write_error("minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		exit(2);
	}
	if (cmd->args[2])
	{
		write_error("minishell: exit: too many arguments\n");
		data->exit_status = 1;
		return (1);
	}
	exit(exit_code % 256);
}
