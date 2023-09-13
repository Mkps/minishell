/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_launcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:21:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/11 13:27:44 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	minishell_inline(t_data *data, char *user_input)
{
	data->user_input = ft_strdup(user_input);
	scan_input(data);
	parse_token(data);
	if (check_error(data) == EXIT_SUCCESS)
	{
		build_cmd_list(data, *data->token_root);
		execute(data);
	}
	dup2(data->old_fd[0], 0);
}

void	minishell_prompt(t_data *data)
{
	while(1)
	{
		signals_interact();
		get_next_line(-1);
		data->user_input = readline("$ ");
		// signals_no_interact();
		if ((data->user_input != NULL && (!strcmp(data->user_input, "exit")) || data->user_input == NULL))
		{
			if (!data->user_input)
				write(1, "\n", 1);
			break ;
		}
		scan_input(data);
		parse_token(data);
		parse_near_quote(data);
		// t_token *tmp = *data->token_root;
		// while (tmp)
		// {
		// 	printf("tmp token value %s | type %i\n", tmp->value, tmp->token_type);
		// 	tmp = tmp->next;
		// }
		if (check_error(data) == EXIT_SUCCESS)
		{
			build_cmd_list(data, *data->token_root);
			execute(data);
		}
		free_data(data);
		dup2(data->old_fd[0], 0);
	}
}
