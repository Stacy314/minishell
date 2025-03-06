/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/05 21:35:39 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// handle herdoc and child (ctrl + c)

//^\Quit (core dumped) (cat  ctrl + /)

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
	printf("Quit: 3\n");
}

void	signal_handler(void)
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
