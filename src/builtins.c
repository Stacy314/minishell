/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/18 20:01:32 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//expansions

//void	builtin_echo(t_cmd *cmd, t_data *data)
//{
//	//need to fix: echo ""
//	int		i;
//	int 	n_flag;
	
//	i = 1;
//	n_flag = 0;
//	while (cmd->args[i] != NULL && is_option(cmd->args[i]))
//	{
//		n_flag = 1;
//		i++;
//	}
//	if (cmd->args[i] != NULL && ft_strncmp(cmd->args[i], "$?", 2) == 0)
//	{
//		printf("%d\n", data->exit_status);
//		return ;
//	}
//	while(cmd->args[i] != NULL)
//	{
//		printf("%s", cmd->args[i]);
//		if (cmd->args[i + 1] != NULL)
//			printf(" ");
//		i++;
//	}
//	if (!n_flag)
//		printf("\n");
//}

void builtin_echo(t_cmd *cmd, t_data *data)
{
    pid_t pid = fork();
    if (pid == -1)
	{
        perror("fork");
        return;
    }
    if (pid == 0)
	{
        int i = 1;
        int n_flag = 0;

        while (cmd->args[i] != NULL && is_option(cmd->args[i]))
		{
            n_flag = 1;
            i++;
        }
        if (cmd->args[i] != NULL && ft_strncmp(cmd->args[i], "$?", 2) == 0)
		{
            printf("%d\n", data->exit_status);
            exit(0);
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
        exit(0);
    } 
    waitpid(pid, NULL, 0);
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

//void	builtin_pwd(t_cmd *cmd)
//{
//	char	*buf;
//	int		i;
	
//	i = 1;
//	if (cmd->args[i] && ft_strchr(cmd->args[i], '-'))
//	{
//		printf("bash: pwd: -%c: invalid option\n", cmd->args[i][1]);
//		return ;
//	}
//	buf = getcwd(NULL, 0);
//	if (buf == NULL)
//	{
//		ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories: ", 2);
//		ft_putstr_fd("\n", 2);
//		return ;
//	}
//	ft_putendl_fd(buf, 1);
//	free(buf);
//	return ;
//}

void builtin_pwd(t_cmd *cmd)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    if (pid == 0)
	{ // Child process
		int i = 1;
		if (cmd->args[i] && ft_strchr(cmd->args[i], '-'))
		{
			printf("bash: pwd: -%c: invalid option\n", cmd->args[i][1]);
			return ;
		}
        char *buf = getcwd(NULL, 0);
        if (buf == NULL) {
            ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories\n", 2);
            exit(1);
        }
        ft_putendl_fd(buf, 1);
        free(buf);
        exit(0);
    }
    waitpid(pid, NULL, 0); // Parent waits for child
}


void builtin_export(t_cmd *cmd, t_data *data)
{
    if (!cmd->args[1]) { // Якщо немає аргументів, вивести всі змінні
        for (int i = 0; data->env[i]; i++) {
            printf("declare -x %s\n", data->env[i]);
        }
        return;
    }

    for (int i = 1; cmd->args[i]; i++) {
        char *arg = cmd->args[i];
        char *equal_sign = strchr(arg, '=');

        if (!equal_sign) {
            printf("minishell: export: `%s': not a valid identifier\n", arg);
            continue;
        }

        int var_index = find_env_var(data->env, arg);
        if (var_index != -1) {
            free(data->env[var_index]);
            data->env[var_index] = strdup(arg);
        } else {
            int env_size;
            for (env_size = 0; data->env[env_size]; env_size++);

            char **new_env = malloc((env_size + 2) * sizeof(char *));
            if (!new_env) {
                perror("malloc");
                return;
            }

            for (int j = 0; j < env_size; j++) {
                new_env[j] = data->env[j];
            }
            new_env[env_size] = strdup(arg);
            new_env[env_size + 1] = NULL;

            free(data->env);
            data->env = new_env;
        }
    }
}

void builtin_unset(t_cmd *cmd, t_data *data)
{
    if (!cmd->args[1])
	{
        printf("minishell: unset: not enough arguments\n");
        return;
    }
    for (int i = 1; cmd->args[i]; i++) {
        int var_index = find_env_var(data->env, cmd->args[i]);
        if (var_index == -1) {
            continue;
        }

        free(data->env[var_index]);
        for (int j = var_index; data->env[j]; j++) {
            data->env[j] = data->env[j + 1];
        }
    }
}

//void	builtin_env(t_cmd *cmd, t_data data)
//{
//	int i;
//	(void)cmd;
//	printf("env run\n");
//	i = 0;
//	while (data.env[i])
//	{
//		ft_putstr_fd(data.env[i], 1);
//		ft_putstr_fd("\n", 1);
//		i++;
//	}
//}

void builtin_env(t_cmd *cmd, t_data *data)
{
	(void)cmd;
    pid_t pid = fork();
    if (pid == -1)
	{
        perror("fork");
        return;
    }
    if (pid == 0)
	{
        int i = 0;
        while (data->env[i]) {
            ft_putstr_fd(data->env[i], 1);
            ft_putstr_fd("\n", 1);
            i++;
        }
        exit(0);
    }
    waitpid(pid, NULL, 0);
}


//after wrong command (127), need to exit twice 
void	builtin_exit(t_cmd *cmd, t_data *data)
{
	int i;
	
	i = 0;
	if (cmd->args[1])
	{
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]))
			{
				printf("exit\n"); ///
				printf("minishell: exit: %s: numeric argument required\n", cmd->args[1]);
				exit(2);
			}
			i++;
		}
		data->exit_status = ft_atoi(cmd->args[1]);
	}
	printf("exit status - %d\n", data->exit_status);
	exit(data->exit_status);
}
