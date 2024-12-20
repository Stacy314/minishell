#include "../minishell.h"

void	builtin_echo(t_data data)
{
	int		i;
	
	i = 1;
	while (data.cmd[i])
	{
		ft_putstr_fd(data.cmd[i], 1);
		if (data.cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
}

void	builtin_cd(t_data data)
{
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
	
	buf = getcwd(NULL, 0);
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("\n", 1);
	free(buf);
}

void	builtin_export(t_data data)
{
	int i;
	
	i = 1;
	while (data.cmd[i])
	{
		if (ft_strchr(data.cmd[i], '='))
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
	//need to change
	data.exit_status = 0;
	exit(0);
}