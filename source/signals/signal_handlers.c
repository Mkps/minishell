/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 16:05:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/05 11:02:03 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	redisplay_prompt(int signum);
void	signal_quit(int signum);
void	signal_nl(int signum);
void	signal_sigint_heredoc(int signum);

void	signals_interact(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_flags = 0;
	act.sa_handler = &redisplay_prompt;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);
}

void	signals_here_doc(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_flags = 0;
	act.sa_handler = &signal_sigint_heredoc;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);
}

void	signals_no_interact(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_flags = 0;
	act.sa_handler = &signal_nl;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = &signal_quit;
	sigaction(SIGQUIT, &act, NULL);
}

void	convert_sig(int signum)
{
	if (signum == SIGINT)
		g_exit_code = 130;
	else if (signum == SIGQUIT)
		g_exit_code = 131;
}

void	signal_parent(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_flags = SA_RESTART;
	act.sa_handler = &convert_sig;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
