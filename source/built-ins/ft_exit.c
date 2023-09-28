#include "../../include/minishell.h"

static int	str_is_digit(char *str)
{
	while (*str)
		if (!ft_isdigit(*str++))
			return (0);
	return (1);
}

int	exit_error(t_data *data, t_cmd *cmd)
{
	int	e_value;

	if (cmd->args[1] && !cmd->args[2])
	{
		write(1, "exit\n", 5);
		if (str_is_digit(cmd->args[1]))
			e_value = ft_atoi(cmd->args[1]);
		else
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
		free_shell(data);
		exit(1);
	}
	return (EXIT_SUCCESS);
}

int	ft_exit(t_data *data, t_cmd *cmd)
{
	if (!cmd->args[1] || (cmd->args[1] && cmd->args[1] == 0))
	{
		write(1, "exit\n", 5);
		free_shell(data);
		exit(data->exit_status);
	}
	else
		exit_error(data, cmd);
	return (EXIT_SUCCESS);
}
