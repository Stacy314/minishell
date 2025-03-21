/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/21 23:00:40 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// exit | exit | exit (shouldn't exit and shouldn't print anything)

// echo test |  <<lala

///bin/env | grep "_=" (32)

///bin/env | grep "SHLVL" (34)

//ls | cat << stop | grep "asd"  (55)
//is this good
//stop

//ls | cat << stop | ls -la | cat << stop1 (59)
//12 
//32232
//23
//stop
//awdaw
//daswd
//stop1

//ls | cat << stop | ls -la | cat << stop1 | ls | cat << stop2 | ls -la | cat << stop3 (68)
//$USER
//ad
//as $HOME
//stop
//awd
//wf$PWDdqwdwqd
//stop1
//das
//das
//stop2
//dsq
//wd
//wf$PWDdqwdwqd
//stop3

//ls | cat << stop | ls -la | cat << stop1 | ls | cat << stop2 | ls -la > out | cat << 'stop3' (84)
//$USER
//ad
//stop
//dsa
//stop1
//sad
//stop2
//as $HOME
//stop3
///bin/rm -f out

// echo hi | echo hi | (should open input)

int	count_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

// pid_t	execute_first_command(t_token **tokens, t_cmd *cmd, t_data *data,
// 		char **env, int pipe_fd[2])
// {
// 	pid_t	pid;

// 	(void)data;
// 	apply_redirections(cmd, data);
// 	if (pipe(pipe_fd) == -1)
// 		return (perror("pipe"), -1);
// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("fork"), -1);
// 	if (pid == 0)
// 	{
// 		//set_child_signals(); //
// 		// dup2(pipe_fd[1], STDOUT_FILENO);
// 		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			exit(1);
// 		}
// 		close(pipe_fd[0]);
// 		close(pipe_fd[1]);
// 		// apply_redirections(cmd, data);
// 		// execute_redirection(cmd, data, env);
// 		execute_for_one(tokens, cmd, data, env);
// 		close(pipe_fd[1]); //
// 		exit(data->exit_status);
// 	}
// 	close(pipe_fd[1]); //
// 	return (pid);
// }

pid_t	execute_first_command(t_token **tokens, t_cmd *cmd, t_data *data,
	char **env, int pipe_fd[2])
{
pid_t	pid;

if (pipe(pipe_fd) == -1)
	return (perror("pipe"), -1);
pid = fork();
if (pid < 0)
	return (perror("fork"), -1);
if (pid == 0)
{
	// apply_redirections(cmd, data);
	// Налаштовуємо пайп для STDOUT
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	apply_redirections(cmd, data);
	execute_for_one(tokens, cmd, data, env);
	exit(data->exit_status);
}
close(pipe_fd[1]);
return (pid);
}


pid_t	execute_middle_command(t_token **tokens, t_cmd *cmd, t_data *data,
		char **env, int in_fd, int new_pipe_fd[2])
{
	pid_t	pid;

	(void)data;
	// apply_redirections(cmd, data);
	if (pipe(new_pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		// apply_redirections(cmd, data);
		// dup2(in_fd, STDIN_FILENO);
		// dup2(new_pipe_fd[1], STDOUT_FILENO);
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit(1);
		}
		if (dup2(new_pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 new_pipe_fd[1]");
			exit(1);
		}
		close(in_fd);          //
		close(new_pipe_fd[0]); //
		close(new_pipe_fd[1]); //
		// execute_redirection(cmd, data, env);
		apply_redirections(cmd, data);
		execute_for_one(tokens, cmd, data, env);
		exit(data->exit_status);
	}
	close(in_fd); //
	close(new_pipe_fd[1]);
	return (pid);
}

pid_t	execute_last_command(t_token **tokens, t_cmd *cmd, t_data *data,
		char **env, int in_fd)
{
	pid_t	pid;

	(void)data;
	// apply_redirections(cmd, data);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		// apply_redirections(cmd, data);
		if (in_fd != -1)
		{
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 in_fd");
				exit(1);
			}
			close(in_fd);
		}
		// execute_redirection(cmd, data, env);
		apply_redirections(cmd, data);
		execute_for_one(tokens, cmd, data, env);
		close(0); //
		exit(data->exit_status);
	}
	if (in_fd != -1)
		close(in_fd);
	// close(0); //
	return (pid);
}

void	execute_pipeline(t_token **tokens, t_cmd *cmd, t_data *data, char **env)
{
	int		n_cmds;
	pid_t	*pids;
	int		pipe_fd[2];
	int		new_pipe_fd[2];
	int		process_count;
	t_cmd	*current;
	int		in_fd;
	int		status;
	int		i;
	//int		sig;

	n_cmds = count_commands(cmd);
	if (n_cmds == 0)
		return ;
	pids = ft_calloc(sizeof(pid_t) * n_cmds, 1);
	if (!pids)
		return ;
	process_count = 0;
	current = cmd;
	in_fd = -1;
	t_cmd	*tmp = cmd;
	while (tmp)
	{
		if (tmp->heredoc_delimiter)
		{
			tmp->heredoc_fd = prepare_heredoc(tmp);
			if (tmp->heredoc_fd == -1)
			{
				return ;
				// Можна встановити помилку або продовжити без here-doc
			}
		}
		tmp = tmp->next;
	}
	pids[process_count++] = execute_first_command(tokens, current, data, env,
			pipe_fd);
	in_fd = pipe_fd[0];
	current = current->next;
	while (current && current->next)
	{
		pids[process_count++] = execute_middle_command(tokens, current, data,
				env, in_fd, new_pipe_fd);
		in_fd = new_pipe_fd[0];
		current = current->next;
	}
	if (current)
		pids[process_count++] = execute_last_command(tokens, current, data, env,
				in_fd);
	else
		close(in_fd);
	// last_exit_status = 0;
	i = 0;
	while (i < process_count)
	{
		waitpid(pids[i], &status, 0);
		 if (WIFEXITED(status))
		{
			///* Katya */
			//if (WEXITSTATUS(status) == 130 || WEXITSTATUS(status) == 131)
			//{
			//	if (WEXITSTATUS(status) == 131)
			//		printf("Quit (core dumped)");
			//	write (1, "\n", 1);
			//}
			///* End */
			data->exit_status = WEXITSTATUS(status);
			// printf("Process %d exited with status %d\n", pids[i],
				//data->exit_status);
		}
		//if (WIFSIGNALED(status))
		//{
		//	sig = WTERMSIG(status);
		//	if (sig == SIGINT)
		//		data->exit_status = 130;
		//	else if (sig == SIGQUIT)
		//	{
		//		// Накшталт bash:
		//		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		//		data->exit_status = 131;
		//	}
		//}
		//else if (WIFEXITED(status))
		//	data->exit_status = WEXITSTATUS(status);
		i++;
	}
	// data->exit_status = last_exit_status;
	// close(0);
	// close(1);
	// close(2);
	// close(3);
	free(pids);
}
