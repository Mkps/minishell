/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:19:43 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/04 14:47:02 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_in_env(t_data *data, char *variable, char **variable_split)
{
	char	*key;
	char	*value;
	t_env	*new_env;

	if (data->flag > 0)
		return (EXIT_FAILURE);
	env_assign(&variable_split, variable, &key, &value);
	if (key_is_valid(key) == 1 || ((value[0] == '\0')
			&& ft_strrchr(variable, '=') == NULL))
		return (free_set_in(key, value, variable_split), EXIT_FAILURE);
	new_env = ft_lstnew_env(key, value);
	if (!new_env)
		return (perror("Malloc failed"),
			free_set_in(key, value, variable_split), EXIT_FAILURE);
	if (env_key_exists(*data->env_cpy, key) == 1)
	{
		if (value[0] != '\0')
			return (envcpy_update_utils(data, new_env), env_update(data),
				free_env_node(new_env), free_set_in(key, value, variable_split),
				EXIT_SUCCESS);
		return (free_env_node(new_env),
			free_set_in(key, value, variable_split), EXIT_FAILURE);
	}
	return (ft_lstadd_back_env(data->env_cpy, new_env),
		free_set_in(key, value, variable_split), EXIT_SUCCESS);
}

int	parse_and_validate_export(char *var, char **key, char **val, int *flag)
{
	char	**variable_split;

	variable_split = ft_split2(var, '=');
	*key = ft_strdup(variable_split[0]);
	*val = ft_strdup(variable_split[1]);
	if (key_is_valid(*key) == 1)
	{
		invalid_export_print(*key, *val, variable_split);
		return (EXIT_FAILURE);
	}
	if (ft_strrchr(var, '=') == NULL)
		(*flag)++;
	if ((*val)[0] != '\0')
		*val = add_quotes(*val);
	if (variable_split)
		ft_free_tab(variable_split);
	return (EXIT_SUCCESS);
}

int	set_in_export(t_data *data, char *var, t_cmd *cmd)
{
	char		*key;
	char		*value;
	int			flag;
	t_export	*new_export;

	flag = 0;
	if (parse_and_validate_export(var, &key, &value, &flag) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (export_key_exists(*data->env_export, key) == 1)
	{
		if (ft_strrchr(var, '=') == NULL)
			data->flag = -1;
		if (set_in_export_utils(data, key, value, cmd) == 0)
			return (EXIT_SUCCESS);
		return (EXIT_FAILURE);
	}
	new_export = ft_lstnew_export(key, value, flag);
	if (!new_export)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	ft_lstadd_back_export(data->env_export, new_export);
	free_set_in(key, value, NULL);
	return (EXIT_SUCCESS);
}

int	set_in_export_utils(t_data *data, char *key, char *value, t_cmd *cmd)
{
	int	err;

	if (value[0] != '\0')
	{
		remove_export(data, key);
		free_set_in(key, value, NULL);
		err = execute_export(data, cmd);
		env_update(data);
		return (err);
	}
	else if (value[0] == '\0' && data->flag != -1)
	{
		remove_export(data, key);
		free_set_in(key, value, NULL);
		err = execute_export(data, cmd);
		env_update(data);
		return (err);
	}
	else
	{
		free_set_in(key, value, NULL);
		return (1);
	}
}

int	execute_export(t_data *data, t_cmd *cmd)
{
	int		i;
	int		err;
	char	**variable_split;

	variable_split = NULL;
	i = 1;
	err = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i] && !is_valid_var(cmd->args[i]))
		{
			printf("export: `%s': not a valid identifier\n", cmd->args[i]);
			err++;
			i++;
		}
		else
		{
			err += set_in_export(data, cmd->args[i], cmd);
			err += set_in_env(data, cmd->args[i], variable_split);
			i++;
		}
	}
	return (err > 0);
}
