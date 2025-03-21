/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastasiia <anastasiia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/21 21:51:03 by anastasiia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//CTRL+C
//CTRL+D
//CTRL+\
//CTRL+C, CTRL+C, CTRL+C, exit
//CTRL+C, CTRL+C, CTRL+C, Ctrl+D

//cat | ls # testar dando depois:
//#- Enter
//#- Ctrl+D
//#- Ctrl+\
//#- Ctrl+C

//grep oi | ls # testar dando depois:
//#- Enter + Ctrl+D
//#- Ctrl+D
//#- "oi" + Enter + Ctrl+D
//#- Ctrl+\
//#- Ctrl+C
//#- "oi" + Enter + Ctrl+\

//./test_files/loop.out # finalizar com:
//#- Ctrl+C
//#- Ctrl+\  -> workspaces: ^\Quit\n
//#- Ctrl+D  -> não faz nada

//./test_files/loop.out | ls
//# finalizar com:
//#- Ctrl+C
//#- Ctrl+\
//#- Ctrl+D

//ls | ./test_files/loop.out
//# finalizar com:
//#- Ctrl+C
//#- Ctrl+\
//#- Ctrl+D

//hello + Ctrl+C
//# Check that the new line is empty


// handle herdoc and child (ctrl + c)

//^\Quit (core dumped) (cat  ctrl + /)

// print minishell twice


//////////////////// ioctl


void	set_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//write(STDOUT_FILENO, "\n", 1); // ???
}

////////////////////////////
static void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		//write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
}

void	set_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

/////////////////////////////////

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_prompt_flag = 1;
	
}

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
	g_prompt_flag = 2;
}

void	signal_handler()
{
	struct sigaction	sa;
	
	// Handle SIGINT (CTRL-C)
	sa.sa_handler = handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	// Handle SIGQUIT (CTRL-\)
	sa.sa_handler = handle_sigquit;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
}



// void	handle_sigint(int sig)
// {
// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	g_signal = sig;
// }

// void	handle_heredoc_sigint(int sig)
// {
// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	g_signal = sig;
// }

// void	handle_child_sigint(int sig)
// {
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	g_signal = sig;
// }

// void	handle_sigquit(int sig)
// {
// 	(void)sig;
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// }

// void	set_signals(int type)
// {
// 	if (type == MAIN)
// 	{
// 		signal(SIGINT, handle_sigint);
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// 	else if (type == CHILD)
// 	{
// 		signal(SIGINT, handle_child_sigint);
// 		signal(SIGQUIT, handle_sigquit);
// 	}
// 	else if (type == HEREDOC)
// 	{
// 		signal(SIGINT, handle_heredoc_sigint);
// 		signal(SIGQUIT, SIG_IGN);
// 	}
// 	else if (type == WAIT)
// 	{
// 		signal(SIGINT, SIG_IGN);
// 		signal(SIGQUIT, handle_sigquit);
// 	}
// }