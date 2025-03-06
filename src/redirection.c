/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/05 16:44:00 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//echo hi | cat <"./test_files/infile"

//cat <"./test_files/infile" | echo hi (print hi)

//cat <"./test_files/infile_big" | echo hi (print hi)

//cat <missing | cat

//cat <missing | echo oi (print oi and error)




void	handle_input_redirect(t_cmd *cmd) // <
{
	int	fd;

	if (!cmd->input_redirect)
		return;

	fd = open(cmd->input_redirect, O_RDONLY);
	if (fd == -1)
	{
		//perror("minishell");
		ft_putstr_fd("minishell: ", 2);  
		ft_putstr_fd(cmd->input_redirect, 2);
		ft_putendl_fd(": No such file or directory", 2); 
		//data->exit_status = 1;
		exit(1); // exit(0); - for |
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		//data->exit_status = 1;
		close(fd);
		exit(1);
	}
	close(fd);
}

void	handle_output_redirect(t_cmd *cmd) // >
{
	int	fd;
	int	i;
	int	arg_index;

	i = 0;
	while (cmd[i].output_redirect)
	{
		fd = open(cmd[i].output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			printf("minishell: %s: No such file or directory\n",
				cmd[i].output_redirect);
			//return (data->exit_status = 1);
		}
		else
		{
			dup2(fd, STDOUT_FILENO);
			arg_index = 1;
			while (cmd->args[arg_index])
			{
				write(fd, cmd->args[arg_index],
						ft_strlen(cmd->args[arg_index])); // if write or read ==
														//-1 return errer
				if (cmd->args[arg_index + 1])
					write(fd, " ", 1);
				arg_index++;
			}
			write(fd, "\n", 1);
			close(fd);
		}
		i++;
	}
}

// echo hi >>4 >>5 >>6 this is a test (should create 3 files(4,5,6) and write into three: hi this is a test (do it twice- and the message should be twice inside))
void	handle_append_redirect(t_cmd *cmd) // >>
{
	int	fd;
	int	i;
	int	arg_index;

	i = 0;
	while (cmd[i].append_redirect)
	{
		fd = open(cmd[i].append_redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			printf("minishell: %s: No such file or directory\n",
				cmd[i].append_redirect);
			//return (data->exit_status = 1);
		}
		else
		{
			dup2(fd, STDOUT_FILENO);
			arg_index = 1;
			while (cmd->args[arg_index])
			{
				write(fd, cmd->args[arg_index],
					ft_strlen(cmd->args[arg_index]));
				if (cmd->args[arg_index + 1])
					write(fd, " ", 1);
				arg_index++;
			}
			write(fd, "\n", 1);
			close(fd);
		}
		i++;
	}
}

void	handle_heredoc(t_cmd *cmd) // <<
{
	int		pipe_fd[2];
	char	*line;
	pid_t	pid;

	if (pipe(pipe_fd) == -1) // ???
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || strcmp(line, cmd->heredoc_delimiter) == 0)
			{
				free(line);
				break ;
			}
			write(pipe_fd[1], line, ft_strlen(line));
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

int	execute_redirection(t_cmd *cmd, t_data *data, char **env)
{
	pid_t	pid;
	int		status;

	(void)env;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pid == 0)
	{
		if (cmd->heredoc_delimiter)
			handle_heredoc(cmd);
		if (cmd->input_redirect)
			handle_input_redirect(cmd);
		if (cmd->output_redirect)
			handle_output_redirect(cmd);
		if (cmd->append_redirect)
			handle_append_redirect(cmd);
		execute_command(cmd->args[0], data, cmd->args, env);
		// execve(cmd->args[0], cmd->args, env);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	return (1);
}
