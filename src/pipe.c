/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/20 16:16:25 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//int	pipe_heandler(t_data data)
//{
//	int pipe_fd[2];
//	int status;
//	int done;
//	pid_t pid;
//	//char *argv1[3] = {"ls", "-l", NULL}; //
//	//char *argv2[3] = {"cat", NULL, NULL}; //
	
//	printf("pipe run\n");
//	pipe(pipe_fd);
//	pid = fork(); 
//	if (pid == 0) //left child
//	{
//		//close(pipe_fd[0]);
//		dup2(pipe_fd[1], STDOUT_FILENO); //make output go to pipe
//		//execlp(argv1[0], argv1[0], argv1[1], (char *)NULL);
//		execve("/bin/ls", (char *[]){"ls", "-l", NULL}, NULL);
//	}
//	pid = fork();
//	if (pid == 0) //right child
//	{
//		close(pipe_fd[1]);
//		dup2(pipe_fd[0], STDIN_FILENO); //make input come from pipe
//		//execlp(argv2[0], argv2[0], (char *)NULL);
//		execve("/bin/cat", (char *[]){"cat", NULL}, NULL);
//	}
//	close(pipe_fd[0]);
//	close(pipe_fd[1]);
	
//	waitpid(-1, &status, 0);
//	waitpid(-1, &status, 0);
//}

// Запускає одну команду з перенаправленням через пайп
void execute_piped_command(t_cmd *cmd, int in_fd, int out_fd, char **env) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) { // Дочірній процес
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execve(cmd->args[0], cmd->args, env);
        perror("execve"); // Якщо execve не вдалося
        exit(127);
    }
}

// Виконує всі команди, розділені `|`
void execute_pipeline(t_cmd **cmd, char **env) {
    int pipe_fd[2];
    int in_fd = STDIN_FILENO;
    int i = 0;
    pid_t last_pid;

    while (cmd[i]) {
        if (cmd[i + 1]) { // Створюємо пайп, якщо не остання команда
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        } else {
            pipe_fd[1] = STDOUT_FILENO; // Остання команда пише у `stdout`
        }

        last_pid = fork();
        if (last_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (last_pid == 0) { // Дочірній процес
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (pipe_fd[1] != STDOUT_FILENO) {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            execve(cmd[i]->args[0], cmd[i]->args, env);
            perror("execve");
            exit(127);
        }

        close(in_fd);
        close(pipe_fd[1]);
        in_fd = pipe_fd[0]; // Новий вхідний fd для наступної команди
        i++;
    }

    // Очікуємо завершення всіх дочірніх процесів
    int status;
    while (waitpid(-1, &status, 0) > 0);
}
