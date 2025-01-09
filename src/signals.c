/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/09 16:40:19 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	// rl_clear_history();
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("Quit: 3\n");
}

//void	signal_handler(void)
//{
//    signal(SIGINT, handle_sigint);   // Handle CTRL-C
//    signal(SIGQUIT, handle_sigquit); // Ignore CTRL-
//}

void signal_handler(void)
{
    struct sigaction sa;

    // Handle SIGINT (CTRL-C)
    sa.sa_handler = handle_sigint;
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // Handle SIGQUIT (CTRL-\)
    sa.sa_handler = handle_sigquit;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGQUIT, &sa, NULL);
}
