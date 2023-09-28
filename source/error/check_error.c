/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:43:23 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:19 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_err(char *input, t_data *data)
{
	int	i;
	int	input_length;

	input = data->user_input;
	data->parse_status = NONE;
	i = 0;
	if (*input == '#')
	{
		add_history(data->raw_input);
		return (EXIT_FAILURE);
	}
	while (*input && ft_is_ws(*input))
		input++;
	if (input == NULL || *input == 0 || *input == '#')
		return (EXIT_FAILURE);
	if (data->raw_input)
	{
		add_history(data->raw_input);
		free(data->raw_input);
		data->raw_input = NULL;
	}
	input_length = ft_strlen(input);
	while (i <= input_length)
		i += ft_get_token_err(input + i, data);
	return (check_error(data));
}

int	check_error_split(t_data *data)
{
	int	i;

	i = -1;
	while (data->cmd_split[++i])
	{
		data->user_input = ft_strdup(data->cmd_split[i]);
		scan_input(data);
		if (check_error(data) != EXIT_SUCCESS)
		{
			free_data(data);
			ft_free_tab(data->cmd_split);
			data->cmd_split = NULL;
			return (1);
		}
		free(data->user_input);
	}
	return (EXIT_SUCCESS);
}

int	check_error_raw(t_data *data)
{
	char	*tmp;

	tmp = ft_strdup(data->raw_input);
	data->raw_input = ft_strdup(data->user_input);
	if (check_err(data->raw_input, data))
	{
		free(tmp);
		free_data(data);
		ft_free_tab(data->cmd_split);
		data->cmd_split = NULL;
		return (1);
	}
	else
	{
		data->raw_input = ft_strdup(data->user_input);
		free_token(data);
		free(data->user_input);
	}
	if (check_error_split(data))
		return (EXIT_FAILURE);
	data->raw_input = tmp;
	free_token(data);
	return (0);
}
