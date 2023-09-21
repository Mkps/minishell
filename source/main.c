/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/21 13:34:04 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

// Get path from the env variable. 
// If envv is (null) set the path to linux defaults
char	**get_path(char **envv)
{
	char	**env_p;
	char	*env;

	env = ft_getenv(envv, "PATH");
	if (env == NULL)
	{
		free(env);
		env = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	}
	env_p = ft_split(env, ':');
	return (env_p);
}

int	arg_check(int ac, char **av)
{
	if (ac != 1 && ac != 3)
	{
		printf("%s\n", USAGE_MSG);
		return (0);
	}
	if (ac == 3 && ft_strncmp(av[1], "-c", 3))
	{
		printf("%s\n", USAGE_MSG);
		return (0);
	}
	return (1);
}

void	print_token(t_token **root)
{
	t_token *current;

	current = *root;
	while (current != NULL)
	{
		printf("token type %i | value %s | qs %i\n", current->token_type, current->value, current->quote_status);
		current = current->next;
	}
}

int	g_exit_code;
int	main(int ac, char **av, char **envv)
{
	t_data	data;
	char	*input;
	pid_t	pid;		
	t_token **tmp;
	t_cmd	*tmp_cmd;
	int		status;
	int		exit_status;

	g_exit_code = 0;
	init_data(&data);
	import_envv(&data, envv);
	copy_env_to_list(&data);
	sort_export_list(&data);

	//print_env_list(data.export);
	t_cmd *cmd = *data.cmd_list;
	if (!arg_check(ac, av))
		return (EXIT_FAILURE);
	if (ac == 3)
		minishell_inline(&data, av[2]);
	else if (ac == 1)
		minishell_prompt(&data);
	free_shell(&data);
	return (g_exit_code);
}
