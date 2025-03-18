/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/18 16:41:03 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// handle herdoc and child (ctrl + c)

//^\Quit (core dumped) (cat  ctrl + /)

// print minishell twice

void	set_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

////////////////////////////
static void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
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
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// g_prompt_flag = 1; //
	// write(STDOUT_FILENO, "\nminishell$ ", 12);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
}

void	signal_handler(void)
{
	struct sigaction	sa;

	// Handle SIGINT (CTRL-C)  (exit code 130)
	sa.sa_handler = handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	// Handle SIGQUIT (CTRL-\) (131 in some case)
	sa.sa_handler = handle_sigquit;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
}
