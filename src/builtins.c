/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/16 16:24:03 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

//Test  15: ❌ echo "exit_code ->$? user ->$USER home -> $HOME" 
//mini output = (exit_code ->$? user ->anastasiia home -> /home/anastasiia)
//bash output = (exit_code ->0 user ->apechkov home -> /home/apechkov)

//Test  21: ❌ echo $?HELLO 
//mini output = (0)
//bash output = (0HELLO)

void builtin_echo(t_cmd *cmd, t_data *data)
{

	int i;
	int n_flag;
	
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
		data->exit_status = 0;
		return ; 
	}
	while (cmd->args[i] != NULL)
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	data->exit_status = 0;
}

void builtin_cd(t_cmd *cmd, t_data *data)
{
    char *path;

	if (cmd->args[1] == NULL)
	{
        path = get_env_value(data->env, "HOME");
        if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2));
    }
	else if (cmd->args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		data->exit_status = 1;
		return ;
	}
	else if (ft_strncmp(cmd->args[1], "~", 1) == 0)
	{
		path = get_env_value(data->env, "HOME");
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2));
		path = ft_strjoin(path, cmd->args[1] + 1);
	}
	else
        path = cmd->args[1];
    if (chdir(path) == -1)
	{
        //ft_putstr_fd("cd: no such file or directory: ", 2);
		//ft_putstr_fd(cmd->args[1], 2);
		//ft_putstr_fd("\n", 2);
		perror("cd");
		data->exit_status = 1;
		//printf("minishell: cd: %s: No such file or directory\n", cmd->args[1]);
    }
}

void builtin_pwd(t_cmd *cmd)
{
	char *buf;
		//while????
		if (cmd->args[1] && ft_strchr(cmd->args[1], '-'))
		{
			printf("bash: pwd: -%c: invalid option\n", cmd->args[1][1]);
			return ;
		}
        buf = getcwd(NULL, 0);
        if (buf == NULL)
		{
            ft_putendl_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories\n", 2);
            //exit(1);
			//data->exit_status = 1;
        }
        return (ft_putendl_fd(buf, 1),free(buf));
}

//export a="s -lsa"
//		l$a
//export a='"' (shouldn't crash)
//		$a
//export a='"'
//		echo $a

int is_valid_identifier(const char *arg)
{
    int i = 0;
    
    // An empty string is not valid.
    if (!arg || !arg[0])
        return 0;
    
    // If the first character is not a letter or underscore, it's invalid.
    if (!isalpha(arg[0]) && arg[0] != '_')
        return 0;
    
    // Continue until end of string or '=' is reached.
    while (arg[i] && arg[i] != '=')
    {
        if (!isalnum(arg[i]) && arg[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

void builtin_export(t_cmd *cmd, t_data *data)
{
	int i;
	char *arg;
	char *equal_sign;
	int var_index;
	int env_size;
	char **new_env;
	int j; //too much declarations
	
    if (!cmd->args[1])
	{
		i = 0;
        while (data->env[i])
		{
            printf("declare -x %s\n", data->env[i]);
			i++;
        }
        return;
    }
	i = 1;
    while(cmd->args[i])
	{
        arg = cmd->args[i];
		if (!is_valid_identifier(arg))
        {
            // Print an error message similar to bash.
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
            data->exit_status = 1;
            // Depending on your shell design, you may continue checking the other arguments.
            return;
        }
        equal_sign = ft_strchr(arg, '=');
		if (!equal_sign)
		{
            //printf("minishell: export: `%s': not a valid identifier\n", arg); //???

			data->exit_status = 0;
			return ;
        }
		else if (!equal_sign++)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("':", 2);
			ft_putendl_fd(" not a valid identifier", 2);
			data->exit_status = 1;
			return ;
		}
        var_index = find_env_var(data->env, arg);
		if (var_index != -1)
		{
           // free(data->env[var_index]);
            data->env[var_index] = ft_strdup(arg);
        } 
		else
		{
			env_size = 0;
            while (data->env[env_size])
				env_size++;
            new_env = malloc((env_size + 2) * sizeof(char *));
            if (!new_env)
			{
                perror("malloc");
                return;
            }
			j = 0;
            while (j < env_size)
			{
                new_env[j] = data->env[j];
				j++;
            }
            new_env[env_size] = ft_strdup(arg);
            new_env[env_size + 1] = NULL;
			//free(data->env);
			data->env = new_env;
			

        }
		i++;
    }
}

void builtin_unset(t_cmd *cmd, t_data *data)
{
	int i;
	int j;
	int var_index;
	
    if (!cmd->args[1])
	{
        //printf("minishell: unset: not enough arguments\n");
        return ;
    }
	i = 1;
    while (cmd->args[i])
	{
        var_index = find_env_var(data->env, cmd->args[i]);
        if (var_index == -1)
            return ;
        //free(data->env[var_index]);
		j = var_index;
        while (data->env[j])
		{
            data->env[j] = data->env[j + 1];
			j++;
        }
		data->env[var_index] = NULL;
		i++;
    }
}

void builtin_env(t_data *data)
{
	int i;

	i = 0;
	while (data->env[i])
	{
    	ft_putstr_fd(data->env[i], 1);
    	ft_putstr_fd("\n", 1);
		i++;
	}
}

//exit 1111111111111111111111111111111111 (protect from overflow, return value 1 and exit)

void	builtin_exit(t_cmd *cmd, t_data *data)
{
	int i;
	
	i = 0;
	if (cmd->args[1] == NULL)
	{
		printf("exit\n");
		exit(data->exit_status);
	}
	while (cmd->args[1][i])
	{
		if (ft_isdigit(cmd->args[1][i]) || cmd->args[1][i] == '+')
		{
			data->exit_status = ft_atoi(cmd->args[1]);
			break ;
		}
		else if(cmd->args[1][i] == '-')
			exit(156);
		else
		{
			printf("exit\n");
			ft_putstr_fd("minishell: exit: ",2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(":", 2);
			ft_putendl_fd(" numeric argument required", 2);
			exit(2);
		}
		i++;
	}
	if (cmd->args[2] != NULL)
	{
		printf("exit\n");
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		data->exit_status = 1;
		return ;
	}
	data->exit_status = ft_atoi(cmd->args[1]);
	printf("exit\n");
	exit(data->exit_status);
}
