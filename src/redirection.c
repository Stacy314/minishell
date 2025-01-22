/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/22 13:17:26 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirections(t_cmd *cmd)
{
    int	fd;
	int	i;
	
	i = 0;
	while (cmd[i].input_redirect)
	{
        fd = open(cmd[i].input_redirect, O_RDONLY);
		if (fd == -1)
		{
            printf("minishell: %s : No such file or directory\n", cmd[i].input_redirect);
		}
		else
		{
		    dup2(fd, STDIN_FILENO);
        	close(fd);
		}
		i++;
    }
	
    //echo hi > a (didn't save hi)
    if (cmd->output_redirect)
	{ 
        fd = open(cmd->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
            //perror("minishell: output redirection");
			printf("minishell: %s : No such file or directory\n", cmd->input_redirect);
            //exit(1);
        }
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
	
	//echo hi >>4 >>5 >>6 this is a test (should create 3 files(4,5,6) and write into three: hi this is a test (do it twice - and the message should be twice inside))
    if (cmd->append_redirect)
	{
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

// cat << EOF
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

    if (pid == 0)
	{
        close(pipe_fd[0]);
        while (1) {
            line = readline("> ");
            if (!line || strcmp(line, cmd->heredoc_delimiter) == 0)
			{
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
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
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

    if (pid == 0)
	{
        if (cmd->heredoc_delimiter)
            handle_heredoc(cmd);
        handle_redirections(cmd);
		//printf("cmd->args[0]: %s\n", cmd->args[0]);
        execve(cmd->args[0], cmd->args, env);
        //perror("execve");
        exit(127);
    }
    waitpid(pid, NULL, 0);
	return (1);
}
