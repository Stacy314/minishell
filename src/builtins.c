/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/21 20:40:52 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int builtin_echo(t_cmd *cmd, t_data *data)
{
	int i;
	int j;
	int n_flag;
	char *arg;

	i = 1;
	n_flag = 0;
	while (cmd->args[i] != NULL && is_option(cmd->args[i]))
	{
    	n_flag = 1;
   		i++;
	}

	while (cmd->args[i] != NULL)
    {
		arg = cmd->args[i];
		j = 0;
		while (arg[j])
		{
			if (arg[j] == '$' && arg[j + 1] == '?') //move to expantion
			{
				printf("%d", data->exit_status);
				j += 2;
			}
			else
			{
				printf("%c", arg[j]);
				//ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
				j++;
			}
		}
		if (cmd->args[i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}

	if (!n_flag)
		printf("\n");  //ft_putstr_fd("\n", STDOUT_FILENO);
	data->exit_status = 0;
	return (0);
}

int builtin_cd(t_cmd *cmd, t_data *data)
{
    char *path;

	if (cmd->args[1] == NULL)
	{
        path = get_env_value(data->env, "HOME");
        if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), data->exit_status = 1);
    }
	else if (cmd->args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		data->exit_status = 1;
		return (1);
	}
	else if (ft_strncmp(cmd->args[1], "~", 1) == 0)
	{
		path = get_env_value(data->env, "HOME");
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), data->exit_status = 1);
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
	return (data->exit_status = 0);
}

int builtin_pwd(t_cmd *cmd/*, t_data *data*/)//ADD!!!
{
	char *buf;
		//while????
		if (cmd->args[1] && ft_strchr(cmd->args[1], '-'))
		{
			printf("bash: pwd: -%c: invalid option\n", cmd->args[1][1]);
			return (/*data->exit_status = */1);//ADD!!!
		}
        buf = getcwd(NULL, 0);
        if (buf == NULL)
		{
            ft_putendl_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories\n", /*data->exit_status = */2);//ADD!!!
            //exit(1);
			//data->exit_status = 1;
        }
        return (ft_putendl_fd(buf, 1),free(buf), /*data->exit_status = */1);//ADD!!!
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

int builtin_export(t_cmd *cmd, t_data *data)
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
        return (1);
    }
	i = 1;
    while(cmd->args[i])
	{
        arg = cmd->args[i];
		if (!is_valid_identifier(arg))
        {
            fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
            data->exit_status = 1;
            return (1);
        }
        equal_sign = ft_strchr(arg, '=');
		if (!equal_sign)
		{
            //printf("minishell: export: `%s': not a valid identifier\n", arg);
			data->exit_status = 0;
			return (1);
        }
		else if (!equal_sign++)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd("':", 2);
			ft_putendl_fd(" not a valid identifier", 2);
			return (data->exit_status = 1);
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
                return (1);
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
	return (0);
}

int builtin_unset(t_cmd *cmd, t_data *data)
{
	int i;
	int j;
	int var_index;
	
    if (!cmd->args[1])
	{
        //printf("minishell: unset: not enough arguments\n");
        return (1);
    }
	i = 1;
    while (cmd->args[i])
	{
        var_index = find_env_var(data->env, cmd->args[i]);
        if (var_index == -1)
            return (1);
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
	return (0);
}

int builtin_env(t_data *data)
{
	int i;

	i = 0;
	while (data->env[i])
	{
    	ft_putstr_fd(data->env[i], 1);
    	ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

//exit 1111111111111111111111111111111111 (protect from overflow, return value 1 and exit)

int	builtin_exit(t_cmd *cmd, t_data *data)
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
			exit(156); //2
		else
		{
			ft_putstr_fd("exit\n",2);
			ft_putstr_fd("minishell: exit: ",2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(":", 2);
			ft_putendl_fd(" numeric argument required", 2);
			data->exit_status = 2;
			return (1);
		}
		i++;
	}
	if (cmd->args[2] &&  cmd->args[2] != NULL)
	{
		ft_putstr_fd("exit\n",2);
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		data->exit_status = 1;
		return (1);
	}
	data->exit_status = ft_atol(cmd->args[1]);
	printf("exit\n");
	exit(data->exit_status);
}
