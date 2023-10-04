/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 22:34:00 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 19:02:16 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	ft_echo(t_cmd *cmd, int i)
{
	int	flag;

	flag = 0;
	if (cmd->args[1] == NULL)
	{
		if (ft_printf("\n") == -1)
			return (perror("minishell: echo"), EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	i = find_first_word(cmd);
	if (i > 1)
		flag++;
	while (cmd->args[i] != NULL)
	{
		if (ft_printf("%s", cmd->args[i]) == -1)
			return (perror("minishell: echo"), EXIT_FAILURE);
		i++;
		if (cmd->args[i] != NULL)
			if (ft_printf(" ") == -1)
				return (perror("minishell: echo"), EXIT_FAILURE);
	}
	if (!flag)
		if (ft_printf("\n") == -1)
			return (perror("minishell: echo"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_data *data)
{
	char	*pwd_value;

	pwd_value = getcwd(NULL, 0);
	(void)data;
	if (pwd_value != NULL)
	{
		if (ft_printf("%s\n", pwd_value) == -1)
			return (perror("minishell: pwd"), free(pwd_value), EXIT_FAILURE);
		free(pwd_value);
		return (EXIT_SUCCESS);
	}
	return (output_err_ret(EXIT_FAILURE, "ERROR Could not find PWD in env",
			""));
}

char	*prompt_pwd(t_data *data)
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
			return (pwd_value);
		}
		current = current->next;
	}
	return (NULL);
}
