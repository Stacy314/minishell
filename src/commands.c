/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/21 14:26:39 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_path_from_env(char **env)
{
	int i;
	
	i = 0;
    while (env[i] != NULL)
	{
        if (strncmp(env[i], "PATH=", 5) == 0)
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

//ls | wc -l
int execute_command(char *cmd, t_data data, char **args)
{
	pid_t pid;
	int status;
	char *path;
	char *executable;
	char **paths;
	int i;
	
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
		}
		paths = split_path(path);
		executable = find_executable(cmd, paths);
		if (!executable)
		{
			fprintf(stderr, "%s: command not found\n", cmd); //need to change
			for (i = 0; paths[i]; i++) free(paths[i]); //need to change
			free(paths);
			exit(127);
		}
		execve(executable, args, data.env);
		//perror("execve");
		for (int i = 0; paths[i]; i++) free(paths[i]); //need to change
		free(paths);
		free(executable);
		exit(1);
	}
	else
	{
        waitpid(pid, &status, 0); // need to check
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
