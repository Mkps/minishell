/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:42:53 by uaupetit          #+#    #+#             */
/*   Updated: 2023/10/04 16:15:28 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_unset(t_data *data, t_cmd *cmd, int i)
{
	t_export	*prev;
	t_export	*current_export;

	while (cmd->args[++i])
	{
		prev = NULL;
		current_export = *data->env_export;
		while (current_export)
		{
			if (ft_strncmp(current_export->key, cmd->args[i],
					ft_strlen(cmd->args[i])) == 0
				&& ft_strlen(current_export->key)
				== ft_strlen(cmd->args[i]))
			{
				if (prev == NULL)
					*data->env_export = current_export->next;
				else
					prev->next = current_export->next;
				export_unset_free(current_export);
				break ;
			}
			prev = current_export;
			current_export = current_export->next;
		}
	}
}

void	execute_env(t_data *data, t_cmd *cmd)
{
	int		i;
	t_env	*prev;
	t_env	*current_export;

	i = 0;
	while (cmd->args[++i])
	{
		prev = NULL;
		current_export = *data->env_cpy;
		while (current_export)
		{
			if (ft_strncmp(current_export->key, cmd->args[i],
					ft_strlen(cmd->args[i])) == 0)
			{
				if (prev == NULL)
					*data->env_cpy = current_export->next;
				else
					prev->next = current_export->next;
				env_unset_free(current_export);
				break ;
			}
			prev = current_export;
			current_export = current_export->next;
		}
	}
}

int	ft_unset(t_data *data)
{
	t_cmd	*cmd_lst;
	int		i;

	i = 0;
	cmd_lst = NULL;
	cmd_lst = find_unset_command(data);
	if (cmd_lst->args[1] == NULL)
		return (EXIT_SUCCESS);
	else
	{
		execute_unset(data, cmd_lst, i);
		execute_env(data, cmd_lst);
		env_update(data);
	}
	return (EXIT_SUCCESS);
}

void	remove_export(t_data *data, const char *key_to_remove)
{
	t_export	*prev;
	t_export	*current_export;

	prev = NULL;
	current_export = NULL;
	current_export = *data->env_export;
	while (current_export)
	{
		if (ft_strncmp(current_export->key, key_to_remove,
				ft_strlen(current_export->key)) == 0)
		{
			if (prev == NULL)
				*data->env_export = current_export->next;
			else
				prev->next = current_export->next;
			free(current_export->key);
			free(current_export->value);
			free(current_export);
			break ;
		}
		prev = current_export;
		current_export = current_export->next;
	}
}

void	remove_env(t_data *data, const char *key_to_remove)
{
	t_env	*prev;
	t_env	*current_env;

	prev = NULL;
	current_env = *data->env_cpy;
	while (current_env)
	{
		if (ft_strncmp(current_env->key, key_to_remove,
				ft_strlen(current_env->key)) == 0)
		{
			if (prev == NULL)
				*data->env_cpy = current_env->next;
			else
				prev->next = current_env->next;
			env_unset_free(current_env);
			break ;
		}
		prev = current_env;
		current_env = current_env->next;
	}
}
