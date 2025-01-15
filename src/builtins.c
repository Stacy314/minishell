/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/15 21:13:55 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_echo(t_cmd *cmd, t_data *data)
{
	//need to fix: echo ""
	int		i;
	int 	n_flag;
	
	i = 1;
	n_flag = 0;
	while (cmd->args[i] != NULL && is_option(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	if (cmd->args[i] != NULL && ft_strncmp(cmd->args[i], "$?", 2) == 0)
	{
		printf("%d\n", data->exit_status);
		return ;
	}
	while(cmd->args[i] != NULL)
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
}


void builtin_cd(t_cmd *cmd, t_data *data)
{
    char *path;

	if (cmd->args[1] == NULL)
	{
        path = get_env_value(data->env, "HOME");
        if (!path)
		{
            fprintf(stderr, "minishell: cd: HOME not set\n");
            return;
        }
    }
	else
	{
        path = cmd->args[1];
    }
    if (chdir(path) == -1)
	{
        ft_putstr_fd("cd: no such file or directory: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("\n", 2);
    }
}

void	builtin_pwd(t_cmd *cmd)
{
	char	*buf;
	int		i;
	
	i = 1;
	if (cmd->args[i] && ft_strchr(cmd->args[i], '-'))
	{
		printf("bash: pwd: -%c: invalid option\n", cmd->args[i][1]);
		return ;
	}
	buf = getcwd(NULL, 0);
	if (buf == NULL)
	{
		ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("\n", 2);
		return ;
	}
	ft_putendl_fd(buf, 1);
	free(buf);
	return ;
}

void	builtin_export(t_cmd *cmd)
{
	int i;
	(void)cmd;
	printf("export run\n");
	i = 1;
	//while (data.cmd[i])
	//{
	//	if (ft_strchr(data.cmd[i], '=')) //need to check
	//	{
	//		ft_putstr_fd("export: not a valid identifier: ", 2);
	//		ft_putstr_fd(data.cmd[i], 2);
	//		ft_putstr_fd("\n", 2);
	//	}
	//	else
	//	{
	//		//add to env
	//	}
	//	i++;
	//}
}

void	builtin_unset(t_cmd *cmd)
{
	int i;
	(void)cmd;
	printf("unset run\n");
	i = 1;
	//while (data.cmd[i])
	//{
	//	//remove from env
	//	i++;
	//}
}

void	builtin_env(t_cmd *cmd, t_data data)
{
	int i;
	(void)cmd;
	printf("env run\n");
	i = 0;
	while (data.env[i])
	{
		ft_putstr_fd(data.env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
}

void	builtin_exit(t_cmd *cmd)
{
	(void)cmd;
	printf("exit run\n");
	
	int i = 0;
	if (cmd->args[1])
	{
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]))
			{
				printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
				exit(255);
			}
			i++;
		}
	}
	//{
	//	if (data.arg[1])
	//	{
	//		data.exit_status = ft_atoi(data.arg[1]);
	//	}
	//}
	//else
	//	data.exit_status = 0;
	exit(0);
}