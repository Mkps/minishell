/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_launcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:21:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/14 18:16:04 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	minishell_inline(t_data *data, char *user_input)
{
	data->user_input = ft_strdup(user_input);
	scan_input(data);
	if (check_error(data) == EXIT_SUCCESS)
	{
		parse_token(data);
		parse_near_quote(data);
		build_cmd_list(data, *data->token_root);
		execute(data);
	}
	dup2(data->old_fd[0], 0);
}

void	set_wc(t_data *data);

char	*glob_home(t_data *data, char *str)
{
	char	*home;
	char	*ret;
	int		i;

	home = get_var(data, "HOME");
	i = 0;
	while (str[i] && home[i] && str[i] == home[i])
		i++;
	if (i > 0)
	{
		ret = ft_strdup(str + (i - 1));
		ret[0] = '~';
		return(ret);
	}
	return (ft_strdup(str));
}
char	*get_session(t_data *data)
{
	char	*tmp;
	char	*ret;
	int		s_idx;
	int		e_idx;

	if ((tmp = get_var(data, "SESSION_MANAGER")) != NULL)
	{
		s_idx = 0;
		while (tmp[s_idx] && tmp[s_idx] != '/')
			s_idx++;
		s_idx++;
		e_idx = s_idx;
		while (tmp[e_idx] && tmp[e_idx] != ':')
			e_idx++;
		ret = ft_strdup(&tmp[s_idx]);
		ret = ft_str_extract_free(ret, (e_idx - s_idx));
		return (ret);
	}	
	else
		return (ft_strdup("localhost"));
}
char	*set_prompt(t_data *data)
{
	char	*prompt;

	prompt = ft_strappend(RED, get_var(data, "USER"), 0);
	prompt = ft_strappend(prompt, "@", 2);
	prompt = ft_strappend(prompt, get_session(data), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, ":", 2);
	prompt = ft_strappend(prompt, RED, 2);
	prompt = ft_strappend(prompt, glob_home(data, get_var(data, "PWD")), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, "$ ", 2);
	return (prompt);
}

void	minishell_prompt(t_data *data)
{
	char	*prompt;

	while(1)
	{
		prompt = set_prompt(data);
		signals_interact();
		data->user_input = readline(prompt);
		signals_no_interact();
		if ((data->user_input != NULL && (!strcmp(data->user_input, "exit")) || data->user_input == NULL))
		{
			if (!data->user_input)
				write(1, "\n", 1);
			break ;
		}
		data->raw_input = data->user_input;
		scan_input(data);
		if (check_error(data) == EXIT_SUCCESS)
		{
			parse_token(data);
			parse_near_quote(data);
			t_token *tmp = *data->token_root;
			while (tmp)       
			{
				printf("tmp token value %s | type %i nq %i\n", tmp->value, tmp->token_type, tmp->near_quote);
				tmp = tmp->next;
			}
			build_cmd_list(data, *data->token_root);
			// set_wc(data);
			execute(data);
		}
		free(prompt);
		free_data(data);
		dup2(data->old_fd[0], 0);
	}
	free(prompt);
}
