/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:51:01 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/23 01:32:09 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <readline/readline.h>

extern int	g_exit_code;

void	redisplay_prompt(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_forced_update_display();
	}
}

void	ignore_sigquit(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}

void	ignore_sigint(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);
}

void	handle_sigint(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = redisplay_prompt;
	sigaction(SIGINT, &act, NULL);
}

void	signals_interact(void)
{
	ignore_sigquit();
	handle_sigint();
}

void	signal_quit(int signum)
{
	write(1, "Quit\n", 5);
	g_exit_code = signum + 128;
}

void	signal_nl(int signum)
{
	write(1, "\n", 1);
	g_exit_code = signum + 128;
}

void	signals_no_interact(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_nl;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = &signal_quit;
	sigaction(SIGQUIT, &act, NULL);
}

void	signal_sigint_heredoc(int signum)
{
	g_exit_code = signum + 128;
    rl_reset_line_state();
    rl_cleanup_after_signal();
    rl_replace_line("",0);
}

void	signals_here_doc(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_sigint_heredoc;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = &signal_quit;
	sigaction(SIGQUIT, &act, NULL);

}
