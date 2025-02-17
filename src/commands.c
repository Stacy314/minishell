/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/02/17 17:42:18 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_path_from_env(char **env)
{
	int i;
	
	i = 0;
    while (env[i] != NULL)
	{
        if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
            return env[i] + 5; // skip PATH
        }
		i++;
    }
    return NULL;
}

char **split_path(const char *path)
{
	char **paths;
	int i;
	char *path_copy;
	char *token;
	
    paths = ft_calloc(128 * sizeof(char *), 1); //why 128?
    path_copy = ft_strdup(path);
    token = strtok(path_copy, ":"); //need to cange
	i = 0;
    while (token)
	{
        paths[i++] = ft_strdup(token);
        token = strtok(NULL, ":"); //need to cange
    }
    paths[i] = NULL;
    free(path_copy);
    return (paths);
}

char *find_executable(const char *cmd, char **paths)
{
	char *full_path;
	int i;
	
    full_path = malloc(1024); //why 1024?
	i = 0;
    while (paths[i] != NULL)
	{
        snprintf(full_path, 1024, "%s/%s", paths[i], cmd); //need to change
        if (access(full_path, X_OK) == 0)
		{
			//printf("full_path = %s\n", full_path);
            return full_path;
        }
		i++;
    }
    free(full_path);
    return (NULL);
}

int check_permissions(char *cmd)
{
    struct stat buf;

    if (stat(cmd, &buf) == -1)
    {
        fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
        return 127;
    }

    if (S_ISDIR(buf.st_mode))
    {
        fprintf(stderr, "minishell: %s: Is a directory\n", cmd);
        return 126;
    }

    if (access(cmd, X_OK) == -1)
    {
        fprintf(stderr, "minishell: %s: Permission denied\n", cmd);
        return 126;
    }

    return 0;
}

//ls | wc -l

int execute_command(char *cmd, t_data *data, char **args, char **env)
{
    pid_t pid;
    int status;
    char *path;
    char *executable;
    char **paths;
    int i;


    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
    {
        int error_code = check_permissions(cmd);
        if (error_code)
            return error_code;  

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return 1;
        }
        if (pid == 0)
        {
            execve(cmd, args, env);
            perror("execve");
            exit(127);
        }
        else
        {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                data->exit_status = WEXITSTATUS(status);
            else
                data->exit_status = 1;
        }
        return data->exit_status;
    }

    path = get_path_from_env(data->env);
    if (!path)
    {
        fprintf(stderr, "Error: PATH not found in environment\n");
        return 127; 
    }

    paths = split_path(path);
    executable = find_executable(cmd, paths);

    if (!executable)
    {
        fprintf(stderr, "%s: command not found\n", cmd);
        for (i = 0; paths[i]; i++)
            free(paths[i]);
        free(paths);
        return 127;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0)
    {
        execve(executable, args, env);
        perror("execve");
        exit(127);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
        else
            data->exit_status = 1;
    }

    for (i = 0; paths[i]; i++)
        free(paths[i]);
    free(paths);
    free(executable);

    return data->exit_status;
}



//// Тестування
//int main(int argc, char **argv, char **env) {
//    if (argc < 2) {
//        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
//        return 1;
//    }

//    execute_command(argv[1], &argv[1], env);
//    return 0;
//}
