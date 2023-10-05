/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_launcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:21:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/05 10:43:06 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Called once at init
void	set_shlevel(t_data *data)
{
	int		shlvl;	
	char	*ret;
	char	*tmp;

	ret = ft_getenv(data->envv, "SHLVL");
	if (!ret)
	{
		tmp = ft_strappend("SHLVL=", ft_itoa(1), 1);
		ft_setenv(data, tmp);
		return ;
	}
	shlvl = ft_atoi(ret);
	tmp = ft_strappend("SHLVL=", ft_itoa(shlvl + 1), 1);
	ft_setenv(data, tmp);
}

void	minishell_core(t_data *data)
{
	int	heredoc_code;

	scan_input(data);
	if (check_error(data) == EXIT_SUCCESS)
	{
		parse_token(data);
		parse_near_quote(data);
		build_cmd_list(data, *data->token_root);
		init_io_redir(data);
		heredoc_code = g_exit_code;
		if (heredoc_code < 128)
			execute(data);
		else
		{
			close_pipes(data->cmd_list, NULL, NULL);
			g_exit_code = heredoc_code;
		}
	}
}

void	prompt_core(t_data *data)
{
	int	i;

	i = -1;
	while (data->cmd_split[++i])
	{
		data->user_input = ft_strdup(data->cmd_split[i]);
		if (i == 1)
		{
			free(data->raw_input);
			data->raw_input = NULL;
		}
		minishell_core(data);
		free_data(data);
		dup2(data->old_fd[0], STDIN_FILENO);
		dup2(data->old_fd[1], STDOUT_FILENO);
	}
}

void	minishell_prompt(t_data *data)
{
	while (1)
	{
		rl_set_signals();
		signals_interact();
		prompt_user(data);
		signals_no_interact();
		if (check_error_raw(data))
			continue ;
		prompt_core(data);
		ft_free_tab(data->cmd_split);
		data->cmd_split = NULL;
	}
}
