/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:35:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/22 17:12:36 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*glob_home(t_data *data, char *str)
{
	char	*home;
	char	*ret;
	int		i;

	home = get_var(data, "HOME");
	if (!home)
		return (strdup(str));
	i = 0;
	while (str[i] && home[i] && str[i] == home[i])
		i++;
	if (str[i] == '/' && i > 0)
	{
		ret = ft_strdup(str + (i - 1));
		ret[0] = '~';
		return (ret);
	}
	return (ft_strdup(str));
}

static char	*get_session(t_data *data)
{
	char	*tmp;
	char	*ret;
	int		s_idx;
	int		e_idx;

	tmp = get_var(data, "SESSION_MANAGER");
	if (tmp != NULL)
	{
		s_idx = 0;
		while (tmp[s_idx] && tmp[s_idx] != '/')
			s_idx++;
		if (s_idx == 0)
			return (ft_strdup("localhost"));
		s_idx++;
		e_idx = s_idx;
		while (tmp[e_idx] && tmp[e_idx] != ':' && tmp[e_idx] != '.')
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
	char	*username;

	username = get_var(data, "USER");
	if (!username || !*username)
		username = "user";
	prompt = ft_strappend(GREEN, username, 0);
	prompt = ft_strappend(prompt, "@", 2);
	prompt = ft_strappend(prompt, get_session(data), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, ":", 2);
	prompt = ft_strappend(prompt, CYAN, 2);
	prompt = ft_strappend(prompt, glob_home(data, get_var(data, "PWD")), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, "\n$ ", 2);
	return (prompt);
}

void	prompt_user(t_data *data)
{
	char	*prompt;

	prompt = set_prompt(data);
	signal(SIGINT, (void (*)(int))redisplay_prompt);
	redisplay_prompt(42, prompt);
	data->user_input = NULL;
	data->raw_input = NULL;
	data->user_input = readline(prompt);
	free(prompt);
	if ((data->user_input != NULL && (!strcmp(data->user_input, "exit"))
			|| data->user_input == NULL))
	{
		if (!data->user_input)
			write(1, "exit\n", 5);
		free_shell(data);
		exit(0);
	}
}
