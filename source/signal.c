#include "../include/minishell.h"
#include <readline/readline.h>

extern int g_exit_code;
void	redisplay_prompt(int signum, void *ptr)
{
	static char	*prompt;

	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_forced_update_display();
	}
	else
	{
		prompt = (char *)ptr;
	}
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
	act.sa_handler = (void (*) (int))redisplay_prompt;
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
	g_exit_code  = signum + 128;
}

void	signal_nl(int signum)
{
	(void)signum;
	g_exit_code  = signum + 128;
}

void	signals_no_interact(void)
{
	struct sigaction act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = &signal_nl;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = &signal_quit;
	sigaction(SIGQUIT, &act, NULL);
}
void	here_doc_SIGINT(int signum)
{
	g_exit_code  = signum + 128;
	// exit(g_exit_code);
}
void	here_doc_child_SIGINT(const int signum, void *ptr)
{
	static int *fd;


	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		close(fd[1]);
		exit(3);
	}
	if (signum == 42)
	{
		fd = (int *)ptr;
	}
}

void	signals_here_doc(void)
{
	struct sigaction act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = &here_doc_SIGINT;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = &signal_quit;
	sigaction(SIGQUIT, &act, NULL);

}

void	signals_here_doc_child(void)
{
	signal(SIGINT, (void (*)(int))signals_here_doc_child);
	struct sigaction act;

	ft_memset(&act, 0, sizeof(act));	
	act.sa_handler = (void (*) (int))&here_doc_child_SIGINT;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = &signal_quit;
	sigaction(SIGQUIT, &act, NULL);

}
