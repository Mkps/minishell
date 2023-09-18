/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_launcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:21:51 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/18 15:13:55 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	minishell_inline(t_data *data, char *user_input)
{
	t_data new_data;

	data->user_input = user_input;
	scan_input(data);
	if (check_error(data) == EXIT_SUCCESS)
	{
		parse_token(data);
		parse_near_quote(data);
		build_cmd_list(data, *data->token_root);
		// var_expand(data);
		/*t_cmd *cmd;*/
		/*cmd = *new_data.cmd_list;*/
		/*while (cmd)*/
		/*{*/
		/*    printf("cmd %s type %i\n", cmd->cmd, cmd->type);*/
		/*    cmd = cmd->next;*/
		/*}*/
		execute(data);
	}
	dup2(data->old_fd[0], 0);
}
void	minishell_subshell(t_data *data, char *user_input)
{
	t_data new_data;

	printf("%s\n", user_input);
	init_data(&new_data);
	import_envv(&new_data, data->envv);
	new_data.user_input = ft_strdup(user_input);
	new_data.raw_input = ft_strdup(new_data.user_input);
	free_data(data);
	scan_input(&new_data);
	if (check_error(&new_data) == EXIT_SUCCESS)
	{
		parse_token(&new_data);
		parse_near_quote(&new_data);
		build_cmd_list(&new_data, *new_data.token_root);
		// var_expand(&new_data);
		/*t_cmd *cmd;*/
		/*cmd = *new_data.cmd_list;*/
		/*while (cmd)*/
		/*{*/
		/*    printf("cmd %s type %i\n", cmd->cmd, cmd->type);*/
		/*    cmd = cmd->next;*/
		/*}*/
		execute(&new_data);
	}
	/*dup2(data->old_fd[0], 0);*/
	exit (g_exit_code);
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
  
	prompt = ft_strappend(GREEN, get_var(data, "USER"), 0);
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
			 /*t_token *tmp = *data->token_root;*/
			 /*while (tmp)       */
			 /*{*/
			 /*    printf("tmp token value %s | type %i nq %i\n", tmp->value, tmp->token_type, tmp->near_quote);*/
			 /*    tmp = tmp->next;*/
			 /*}*/
			build_cmd_list(data, *data->token_root);
			// var_expand(data);
			execute(data);
		}
		free(prompt);
		free_data(data);
		dup2(data->old_fd[0], 0);
	}
	free(prompt);
}
