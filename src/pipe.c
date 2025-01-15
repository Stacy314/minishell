///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   pipe.c                                             :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
///*   Updated: 2025/01/13 13:38:45 by apechkov         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

//#include "../minishell.h"

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
