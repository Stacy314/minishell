/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/04/10 22:13:01 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern sig_atomic_t	g_signal_flag;

void	handle_sigquit_child(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
}

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

void	handle_sigint(int sig)
{
	g_signal_flag = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals_main(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
