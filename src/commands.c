/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/20 15:55:06 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_path_from_env(char **env)
{
    for (int i = 0; env[i] != NULL; i++)
	{
        if (strncmp(env[i], "PATH=", 5) == 0)
		{
            return env[i] + 5; // Пропустити "PATH="
        }
    }
    return NULL;
}

char **split_path(const char *path)
{
    char **paths = malloc(128 * sizeof(char *));
    int i = 0;
    char *path_copy = strdup(path);
    char *token = strtok(path_copy, ":");

    while (token) {
        paths[i++] = strdup(token);
        token = strtok(NULL, ":");
    }
    paths[i] = NULL;
    free(path_copy);
    return paths;
}

char *find_executable(const char *cmd, char **paths)
{
    char *full_path = malloc(1024);
    for (int i = 0; paths[i] != NULL; i++) {
        snprintf(full_path, 1024, "%s/%s", paths[i], cmd);
        if (access(full_path, X_OK) == 0) {
			//printf("full_path = %s\n", full_path);
            return full_path;
        }
    }
    free(full_path);
    return NULL;
}

int execute_command(char *cmd, t_data data, char **args)
{
	//ls | wc -l
	pid_t pid;
	int status;
	char *path;
	char *executable;
	char **paths;
	(void)data;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		path = get_path_from_env(data.env);
		if (!path)
		{
			fprintf(stderr, "Error: PATH not found in environment\n");
			exit(127); 
			//data->exit_status = 127;
			//return (0);
		}
		paths = split_path(path);
		executable = find_executable(cmd, paths);
		if (!executable)
		{
			fprintf(stderr, "%s: command not found\n", cmd);
			for (int i = 0; paths[i]; i++) free(paths[i]);
			free(paths);
			exit(127);
			//return (0);
		}
		execve(executable, args, data.env);
		perror("execve");
		for (int i = 0; paths[i]; i++) free(paths[i]);
		free(paths);
		free(executable);
		exit(1);
		//return (0);
	}
	else
	{ // Parent process
        waitpid(pid, &status, 0); // Wait for the child process to finish
        if (WIFEXITED(status))
		{
            //printf("Command exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Command did not exit normally\n");
        }
   
	}
	return (1);
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
