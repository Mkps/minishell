/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:35:15 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 15:28:53 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*glob_home(t_data *data, char *str)
{
	char	*home;
	char	*ret;
	int		i;

	home = ft_getenvcpy(data, "HOME");
	if (!str)
		return (ft_strdup("???"));
	if (!home || !*home)
		return (strdup(str));
	i = 0;
	while (str[i] && home[i] && str[i] == home[i])
		i++;
	if ((str[i] == '/' || (!home[i] && !str[i])) && i > 0)
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

	tmp = ft_getenvcpy(data, "SESSION_MANAGER");
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
	char	*pwd;

	username = ft_getenvcpy(data, "USER");
	if (!username || !*username)
		username = "user";
	prompt = ft_strappend(GREEN, username, 0);
	prompt = ft_strappend(prompt, "@", 2);
	prompt = ft_strappend(prompt, get_session(data), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, ":", 2);
	prompt = ft_strappend(prompt, CYAN, 2);
	pwd = prompt_pwd(data);
	prompt = ft_strappend(prompt, glob_home(data, pwd), 3);
	prompt = ft_strappend(prompt, RESET, 2);
	prompt = ft_strappend(prompt, "\n$ ", 2);
	return (prompt);
}

void	prompt_user(t_data *data)
{
	char	*prompt;

	prompt = set_prompt(data);
	data->user_input = NULL;
	data->raw_input = NULL;
	data->user_input = readline(prompt);
	free(prompt);
	if (((data->user_input != NULL && (!strcmp(data->user_input, "exit")))
			|| data->user_input == NULL))
		if (!data->user_input)
			data->user_input = ft_strdup("exit");
	if (g_exit_code > 127)
	{
		data->exit_status = g_exit_code;
		g_exit_code = 0;
	}
	data->raw_input = data->user_input;
	data->cmd_split = ft_split_noquote(data->user_input, ';');
}
