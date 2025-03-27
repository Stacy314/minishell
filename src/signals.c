/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/27 17:11:48 by apechkov         ###   ########.fr       */
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

//bash: warning: here-document at line 7 delimited by end-of-file (heredoc ctrl+D)


static struct sigaction g_old_int; //forbidden
static struct sigaction g_old_quit; //forbidden

void parent_ignore_signals(void)
{
    struct sigaction sa_ignore;

    sa_ignore.sa_handler = SIG_IGN;
    sa_ignore.sa_flags = 0;
    sigemptyset(&sa_ignore.sa_mask);
    sigaction(SIGINT, NULL, &g_old_int);
    sigaction(SIGQUIT, NULL, &g_old_quit);
    sigaction(SIGINT, &sa_ignore, NULL);
    sigaction(SIGQUIT, &sa_ignore, NULL);
}

void parent_restore_signals(void)
{
    sigaction(SIGINT, &g_old_int, NULL);
    sigaction(SIGQUIT, &g_old_quit, NULL);
}


////////////////////////////
void	handle_sigint_child(int sig)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal_flag = sig;
}

// void	handle_sigquit_child(int sig)
// {
// 	(void)sig;
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// }
// void	handle_sigquit_child(int sig)
// {
// 	(void)sig;
// 	printf("Quit (core dumped)\n");  //ioctl
// 	g_signal_flag = sig;
// }


void	set_signals_child(void) // need to fix
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//write(STDOUT_FILENO, "\n", 1); // ???

	//signal(SIGINT, handle_sigint_child); // SIGINT = 2
	//signal(SIGQUIT, handle_sigquit_child); // SIGQUIT = 3
}

////////////////////////////
static void	handle_sigint_heredoc(int sig) //need to fix
{
	// fprintf(stderr, "[DEBUG] handle_sigint heredoc start\n");
	write(STDOUT_FILENO, "\n", 1);  //ioctl
	// exit(130);
	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_flag = sig;
	// fprintf(stderr, "[DEBUG] handle_sigint heredoc end\n");
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

/////////////////////////////////

void	handle_sigint(int sig)
{
	// fprintf(stderr, "[DEBUG] handle_sigint parent start\n");
	// (void)sig;
	write(STDOUT_FILENO, "\n", 1); //ioctl
	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal_flag = sig; // 128 + 2 (SIGINT) = 130
	// fprintf(stderr, "[DEBUG] handle_sigint parent end\n");
}

// void	handle_sigquit(int sig)
// {
// 	(void)sig;
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

void	set_signals_main()
{
	// struct sigaction	sa;

	// // Handle SIGINT (CTRL-C)
	// sa.sa_handler = handle_sigint;
	// sa.sa_flags = SA_RESTART;
	// sigemptyset(&sa.sa_mask);
	// sigaction(SIGINT, &sa, NULL);
	// // Handle SIGQUIT (CTRL-\)
	// sa.sa_handler = handle_sigquit;
	// sa.sa_flags = SA_RESTART;
	// sigemptyset(&sa.sa_mask);
	// sigaction(SIGQUIT, &sa, NULL);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
////////////////////////////////////////////////////////////////////////////////////
