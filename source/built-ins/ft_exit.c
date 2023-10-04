/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 10:30:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 17:55:42 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <limits.h>
#include <values.h>

static int	str_is_digit(char *str)
{
	while (*str)
		if (!ft_isdigit(*str++))
			return (0);
	return (1);
}

long	ft_atol(const char *str)
{
	unsigned long	atol;
	int				sign;
	int				i;

	atol = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (sign == -1)
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		atol = (atol * 10) + str[i] - 48;
		if (atol > (LONG_MAX))
			return (-2);
		i++;
	}
	return (atol);
}

int	exit_error(t_data *data, t_cmd *cmd)
{
	long	e_value;

	if (cmd->args[1] && !cmd->args[2])
	{
		e_value = ft_atol(cmd->args[1]);
		if (!str_is_digit(cmd->args[1]) || e_value <= -1)
		{
			e_value = 2;
			output_err_cmd("numeric arguments required", "exit");
		}
		free_shell(data);
		exit(e_value);
	}
	else if (cmd->args[2])
	{
		output_err_cmd("too many arguments", "exit");
		return (2);
	}
	return (EXIT_SUCCESS);
}

int	ft_exit(t_data *data, t_cmd *cmd)
{
	if (is_standalone(cmd))
		write(1, "exit\n", 5);
	if (!cmd->args[1] || (cmd->args[1] && cmd->args[1] == 0))
	{
		free_shell(data);
		exit(data->exit_status);
	}
	else
		return (exit_error(data, cmd));
	return (EXIT_SUCCESS);
}
