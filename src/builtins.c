/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/09 16:57:16 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_echo(t_data data)
{
	int		i;
	
	i = 1;
	printf("echo run\n");
	//data.arg = .... ;
	while (data.arg[i])
	{
		ft_putstr_fd(data.arg[i], 1);
		i++;
	}
	if (ft_strncmp(data.arg[i], "$?", 2)) //need to check
		ft_putnbr_fd(data.exit_status, 1);
	ft_putstr_fd("\n", 1);
	if(data.option)
	{
		//logic
	}
}

void	builtin_cd(t_data data)
{
	printf("cd run\n");
	if (data.cmd[1] == NULL)
	{
		chdir(data.env[1]);
	}
	else
	{
		if (chdir(data.cmd[1]) == -1)
		{
			ft_putstr_fd("cd: no such file or directory: ", 2);
			ft_putstr_fd(data.cmd[1], 2);
			ft_putstr_fd("\n", 2);
		}
	}
}

void	builtin_pwd(t_data data)
{
	char *buf;
	
	printf("pwd run\n");

	if (data.option) //is it ok? 
	{
		ft_putendl_fd("bash: pwd: -a: invalid option", 2);
		ft_putendl_fd("pwd: usage: pwd [-LP]", 2);
		return ;
	}
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories: ", 2);
		//ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	ft_putendl_fd(buf, 1);
	free(buf);
	return ;
}

void	builtin_export(t_data data)
{
	int i;
	
	printf("export run\n");
	i = 1;
	while (data.cmd[i])
	{
		if (ft_strchr(data.cmd[i], '=')) //need to check
		{
			ft_putstr_fd("export: not a valid identifier: ", 2);
			ft_putstr_fd(data.cmd[i], 2);
			ft_putstr_fd("\n", 2);
		}
		else
		{
			//add to env
		}
		i++;
	}
}

void	builtin_unset(t_data data)
{
	int i;
	
	printf("unset run\n");
	i = 1;
	while (data.cmd[i])
	{
		//remove from env
		i++;
	}
}

void	builtin_env(t_data data)
{
	int i;
	
	printf("env run\n");
	i = 0;
	while (data.env[i])
	{
		ft_putstr_fd(data.env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
}

void	builtin_exit(t_data data)
{
	printf("exit run\n");
	//need to change
	if (data.arg)
	{
		if (data.arg[1])
		{
			data.exit_status = ft_atoi(data.arg[1]);
		}
	}
	else
		data.exit_status = 0;
	exit(0);
}