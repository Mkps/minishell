/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:21:58 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/02 19:16:58 by aloubier         ###   ########.fr       */
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
		return (NULL);
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
	t_token	*current;

	current = *root;
	while (current != NULL)
	{
		printf("token type %i | value %s | qs %i | nq %i\n", current->token_type,
			current->value, current->quote_status, current->near_quote);
		current = current->next;
	}
}

int	g_exit_code;

int	main(int ac, char **av, char **envv)
{
	t_data	data;

	if (!arg_check(ac, av))
		return (EXIT_FAILURE);
	g_exit_code = 0;
	init_data(&data);
	import_envv(&data, envv);
	set_shlevel(&data);
	copy_env_to_list(&data);
	env_to_export(&data);
	sort_export_list(&data);
	if (ac == 3)
		minishell_inline(&data, av[2]);
	else if (ac == 1)
		minishell_prompt(&data);
	free_shell(&data);
	return (g_exit_code);
}
