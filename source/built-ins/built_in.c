/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 22:34:00 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 11:36:28 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

int	ft_env(t_data *data)
{
	return (print_envp(data));
}

int	find_first_word(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	while (cmd->args[i] && cmd->args[i][0] == '-')
	{
		j = 1;
		while (cmd->args[i][j] && cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] == '\0')
			i++;
		else
			break ;
	}
	return (i);
}

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	flag;

	flag = 0;
	if (cmd->args[1] == NULL)
	{
		printf("\n");
		return (EXIT_SUCCESS);
	}
	i = find_first_word(cmd);
	if (i > 1)
		flag++;
	while (cmd->args[i] != NULL)
	{
		ft_printf("%s", cmd->args[i]);
		i++;
		if (cmd->args[i] != NULL)
			ft_printf(" ", cmd->args[i]);
	}
	if (!flag)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_data *data)
{
	char	*pwd_value;
	t_env	*current;

	pwd_value = NULL;
	current = *data->env_cpy;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, "PWD", 3) == 0)
		{
			pwd_value = current->value;
			printf("%s\n", pwd_value);
			return (EXIT_SUCCESS);
		}
		current = current->next;
	}
	return (output_err_ret(EXIT_FAILURE, "ERROR Could not find PWD in env",
			""));
}

int	ft_exit(t_data *data, t_cmd *cmd)
{
	if (!cmd->args[1] || (cmd->args[1] && cmd->args[1] == 0))
	{
		write(1, "exit\n", 5);
		free_shell(data);
		exit(data->exit_status);
	}
	return (EXIT_SUCCESS);
}
