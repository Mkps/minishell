/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:51:01 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/25 18:34:55 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <readline/readline.h>

extern int	g_exit_code;

void	redisplay_prompt(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_reset_line_state();
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_forced_update_display();
	}
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

void	signal_sigint_heredoc(int signum)
{
	g_exit_code = signum + 128;
	rl_reset_line_state();
	rl_replace_line("",0);
	rl_cleanup_after_signal();
	rl_on_new_line();
	rl_forced_update_display();
	if (signum == SIGINT)
	{
		//write(1, "\n", 1);
		//rl_reset_line_state();
		//rl_replace_line("", 0);
		//rl_on_new_line();
		//rl_forced_update_display();
	}
}

