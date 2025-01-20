/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/20 17:28:38 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_redirections(t_cmd *cmd) {
    int fd;

    if (cmd->input_redirect) { // Читаємо з файлу (`<`)
        fd = open(cmd->input_redirect, O_RDONLY);
		if (fd == -1) {
            printf("minishell: %s : No such file or directory\n", cmd->input_redirect);
			//fprintf(stderr, "minishell: %s: ", cmd->input_redirect);
			//perror(""); // `perror("")` просто виведе `strerror(errno)`

            //exit(1);
		}
		else
		{
		    dup2(fd, STDIN_FILENO);
        	close(fd);
		}

    }
    //echo hi > a
	//need to rewrite
    if (cmd->output_redirect) { // Запис у файл (`>`)
        fd = open(cmd->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            //perror("minishell: output redirection");
			printf("minishell: %s : No such file or directory\n", cmd->input_redirect);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
	
	//need to rewrite
    if (cmd->append_redirect)
	{ // Дописування у файл (`>>`)
        fd = open(cmd->append_redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1) {
            //perror("minishell: append redirection");
			printf("minishell: %s : No such file or directory\n", cmd->input_redirect);
            //exit(1);
        }
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

    }
}

// Читає Heredoc (`<<`) до `delimiter`
void handle_heredoc(t_cmd *cmd) {
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process → читає ввід користувача
        close(pipe_fd[0]); // Закриваємо читання
        while (1) {
            line = readline("> ");
            if (!line || strcmp(line, cmd->heredoc_delimiter) == 0) {
                free(line);
                break;
            }
            write(pipe_fd[1], line, strlen(line));
            write(pipe_fd[1], "\n", 1);
            free(line);
        }
        close(pipe_fd[1]);
        exit(0);
    }

    waitpid(pid, NULL, 0);
    close(pipe_fd[1]); // Закриваємо запис
    dup2(pipe_fd[0], STDIN_FILENO); // Читаємо з `pipe`
    close(pipe_fd[0]);
}

//< nonexist | < exists | < nonexistent
int execute_redirection(t_cmd *cmd, char **env)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return (0);
    }

    if (pid == 0) { // Child process
        if (cmd->heredoc_delimiter) // Якщо є `<<`
            handle_heredoc(cmd);
        handle_redirections(cmd); // Обробка `<`, `>`, `>>`
		//printf("cmd->args[0]: %s\n", cmd->args[0]);
        execve(cmd->args[0], cmd->args, env);
        perror("execve");
        exit(127);
    }

    waitpid(pid, NULL, 0); // Parent process чекає
	return (1);
}
