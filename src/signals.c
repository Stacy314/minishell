/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 19:01:20 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


extern sig_atomic_t	g_signal_flag;
//CTRL+C
//CTRL+D
//CTRL+backslash
//CTRL+C, CTRL+C, CTRL+C, exit
//CTRL+C, CTRL+C, CTRL+C, Ctrl+D

//cat | ls # testar dando depois:
//#- Enter
//#- Ctrl+D
//#- Ctrl+backslash
//#- Ctrl+C

//grep oi | ls # testar dando depois:
//#- Enter + Ctrl+D
//#- Ctrl+D
//#- "oi" + Enter + Ctrl+D
//#- Ctrl+
//#- Ctrl+C
//#- "oi" + Enter + Ctrl+backslash

//./test_files/loop.out # finalizar com:
//#- Ctrl+C
//#- Ctrl+\  -> workspaces: ^\Quit\n
//#- Ctrl+D  -> não faz nada

//./test_files/loop.out | ls
//# finalizar com:
//#- Ctrl+C
//#- Ctrl+backslash
//#- Ctrl+D

//ls | ./test_files/loop.out
//# finalizar com:
//#- Ctrl+C
//#- Ctrl+backslash
//#- Ctrl+D

//hello + Ctrl+C
//# Check that the new line is empty


// handle herdoc and child (ctrl + c)

//^\Quit (core dumped) (cat  ctrl + /)

// print minishell twice

//bash: warning: here-document at line 7 delimited by end-of-file (heredoc ctrl+D)
///////
void	parent_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

//////

void	parent_restore_signals(void)
{
	signal(SIGINT, SIG_DFL);
	//signal(SIGQUIT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

////////////////////////////
void handle_sigquit_child(int sig)
{
	//rl_on_new_line();
	(void)sig;
	//g_signal_flag = sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	//write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	
}

//void	handle_sigquit(int sig)
//{
//	//(void)sig;
//	g_signal_flag = sig;
//	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
//	rl_on_new_line();
//	rl_replace_line("", 0);
	
//}
void	handle_sigint_child(int sig)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal_flag = sig;
}

void	set_signals_child(void)
{
	signal(SIGINT, handle_sigint_child);
	signal(SIGQUIT, handle_sigquit_child);
}

////////////////////////////
void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_flag = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	//exit(130);
	//rl_redisplay();
	//exit(130);
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/////////////////////////////////

void	handle_sigint(int sig)
{
	g_signal_flag = sig;
	write(STDOUT_FILENO, "\n", 1); //ioctl
	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals_main()
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
////////////////////////////////////////////////////////////////////////////////////
