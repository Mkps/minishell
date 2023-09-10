#include "../include/minishell.h"

void	redisplay_prompt(int signum)
{
	(void)signum;

	write(1, "\n", 1);
}
void	ignore_sigquit(void)
{
	struct sigaction act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}

void	ignore_sigint(void)
{
	struct sigaction act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, NULL);
}

void	handle_sigint(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = &redisplay_prompt;
	sigaction(SIGINT, &act, NULL);
}

void	signals_interact(void)
{
	ignore_sigquit();
	handle_sigint();
}

void	signal_nl(int signum)
{
	(void)signum;
	write(1, "\n", 1);
}

void	signals_no_interact(void)
{
	struct sigaction act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = &signal_nl;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
