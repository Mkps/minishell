/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 22:34:00 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/22 22:34:02 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env(t_data *data)
{
	return (print_envp(data));
}

int	find_first_word(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 1;
	while (cmd->args[i][0] == '-')
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

	i = 1;
	flag = 0;
	i = find_first_word(cmd);
	while (cmd->args[i] != NULL)
	{
		printf("%s", cmd->args[i]);
		i++;
		if (cmd->args[i] != NULL)
			ft_printf("%s", cmd->args[i]);
	}
	if (!flag)
		printf("\n");
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
	return (output_err_ret(EXIT_FAILURE,
			"ERROR Could not find PWD in env", ""));
}
