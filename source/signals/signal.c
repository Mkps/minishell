/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:51:01 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/29 15:53:27 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/readline.h>

extern int	g_exit_code;

void	redisplay_prompt(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_code = signum + 128;
		rl_echo_signal_char(signum);
		printf("\n");
		rl_reset_line_state();
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_quit(int signum)
{
	g_exit_code = signum + 128;
	write(1, "Quit\n", 5);
}

void	signal_nl(int signum)
{
	g_exit_code = signum + 128;
	printf("\n");
}

void	signal_sigint_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_exit_code = signum + 128;
		rl_echo_signal_char(signum);
		printf("\n");
		rl_reset_line_state();
		rl_free_line_state();
		rl_cleanup_after_signal();
		rl_replace_line("", 0);
	}
}

void	set_sigign(void)
{
	signal(SIGPIPE, SIG_DFL);
	signals_no_interact();
}
