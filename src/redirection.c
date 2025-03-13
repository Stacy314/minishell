/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/13 21:03:57 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//stderr: Broken pipe:
//echo hi | cat <"./test_files/infile"
//cat <"./test_files/infile" | echo hi 


int check_error(const char *filename)
{
    struct stat st;
    if (stat(filename, &st) == -1)
    {
        write_error("minishell: %s: No such file or directory\n", filename);
        return 1; // код помилки (bash зазвичай дає 127 для "No such file or directory")
    }
    // Перевіряємо, чи це директорія
    if (S_ISDIR(st.st_mode)) //do we need this?
    {
        write_error("minishell: %s: Is a directory\n", filename);
        return 1; // bash використовує 126, коли файл існує, але не підходить (наприклад, директорія)
    }
    // Перевіряємо права доступу на читання (для випадку "< filename")
    if (access(filename, R_OK) == -1)
    {
        write_error("minishell: %s: Permission denied\n", filename);
        return 1;
    }
    return 0; // Все добре, можна читати
}

void	handle_input_redirect(t_cmd *cmd) // <
{
	int fd;
	int i;

	if (!cmd->input_redirects)
		return ;
	i = 0;
	int ret = check_error(cmd->input_redirects[i]);
	if (ret != 0)
		exit(1);

	while (cmd->input_redirects[i])
	{
		fd = open(cmd->input_redirects[i], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			exit(1);
			
		}
		// else
		// {
			dup2(fd, STDIN_FILENO);
			close(fd);
		// }
		i++;
	}
	close(fd);
}

void	handle_output_redirect(t_cmd *cmd) // >
{
	int fd;
	int i;

	if (!cmd->output_redirects)
		return ;
	i = 0;
	// // printf("we are here\n");
	// int ret = check_error(cmd->output_redirects[i]);
	// if (ret != 0)
	// {
	// 	//printf("we are here\n");
	// 	exit(ret);
	// }
	// if (access(cmd->output_redirects[i], R_OK) == -1)
    // {
    //     write_error("minishell: %s: Permission denied\n", cmd->output_redirects[i]);
    //     exit (1);
    // }
	//i = 0;
	while (cmd->output_redirects[i])
	{
		fd = open(cmd->output_redirects[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open");
			// write_error("minishell: %s: No such file or directory\n", cmd->output_redirects[i]);
			exit(1);
		}
		// else
		// {
			dup2(fd, STDOUT_FILENO);
			close(fd);
		// }
		i++;
	}
}

void	handle_append_redirect(t_cmd *cmd) // >>
{
	int fd;
	int i;

	if (!cmd->append_redirects)
		return ;
	i = 0;
	// int ret = check_error(cmd->append_redirects[i]);
	// if (ret != 0)
	// 	exit(ret);
	// if (access(cmd->append_redirects[i], R_OK) == -1)
    // {
    //     write_error("minishell: %s: Permission denied\n", cmd->append_redirects[i]);
    //     exit (1);
    // }
	//i = 0;
	while (cmd->append_redirects[i])
	{
		fd = open(cmd->append_redirects[i], O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if (fd == -1)
		{
			perror("open");
			// write_error("minishell: %s: No such file or directory\n", cmd->append_redirects[i]);
			exit(1);
		}
		// else
		// {
			dup2(fd, STDOUT_FILENO);
			close(fd);
		// }
		i++;
	}
}



int	execute_redirection(t_cmd *cmd, t_data *data, char **env, char	*input)
{
	pid_t	pid;
	int		status;
	int		i;

	//(void)env;
	//(void)data;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		i = 0;
		while(input[i])
		{
			if (input[i] == '<') //need to change
			{
				handle_input_redirect(cmd);
			}
			if (input[i] == '>') //need to change
			{
				handle_output_redirect(cmd);
			}
			if (input[i] == '>') //need to change
			{
				handle_append_redirect(cmd);
			}
			//need add heredoc
			i++;
		}
		// if (cmd->heredoc_delimiter)
		// 	handle_heredoc(cmd);
		// if (cmd->input_redirects)
		// 	handle_input_redirect(cmd);
		// if (cmd->output_redirects)
		// 	handle_output_redirect(cmd);
		// if (cmd->append_redirects)
		// 	handle_append_redirect(cmd);
		execute_command(cmd->args[0], data, cmd->args, env);
		exit(data->exit_status); //
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		// printf("exit status: %d\n", WEXITSTATUS(status));	
		data->exit_status = WEXITSTATUS(status);
	}
	//data->exit_status = 0;
	return (1);
}

void	apply_redirections(t_cmd *cmd)
{
	if (cmd->heredoc_delimiter) //need to change with input
		handle_heredoc(cmd);
	if (cmd->input_redirects)
		handle_input_redirect(cmd);
	if (cmd->output_redirects)
		handle_output_redirect(cmd);
	if (cmd->append_redirects)
		handle_append_redirect(cmd);
}
