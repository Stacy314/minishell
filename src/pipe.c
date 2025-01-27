/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/27 18:55:23 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//exit | exit | exit (shouldn't exit and shouldn't print anything)
//export GHOST=123 | env | grep GHOST (exit status 1)
//| echo hi  (bash: syntax error near unexpected token `|')

void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
{
    int pipe_fd[2];
    int in_fd = STDIN_FILENO;
    int i = 0;
    pid_t last_pid;

	//printf("execute_pipeline\n");
	if (!cmd[0] || cmd[0]->args == NULL) // Якщо немає команд після `|`
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
        data->exit_status = 1;
        return ;
    }
	
    while (cmd[i])
	{
		if (cmd[i + 1])
		{
            if (pipe(pipe_fd) == -1)
			{
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
		else
		{
            pipe_fd[1] = STDOUT_FILENO; // Остання команда пише у `stdout`
        }
		printf("%s\n", cmd[i]->args[0]);
        last_pid = fork();
        if (last_pid == -1)
		{
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (last_pid == 0)
		{
            if (in_fd != STDIN_FILENO)
			{
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (pipe_fd[1] != STDOUT_FILENO)
			{
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
			if (cmd[i]->args[0] && strcmp(cmd[i]->args[0], "exit") == 0) {
                exit(0); // 🔹 `exit` у пайпі просто завершує дочірній процес
            }
            execve(cmd[i]->args[0], cmd[i]->args, env);
            //perror("execve");
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
	if (WIFEXITED(status))
        data->exit_status = WEXITSTATUS(status);
}
