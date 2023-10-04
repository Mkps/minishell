/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:19:35 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/02 14:01:54 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*build_wc_cd(char *f_wc)
{
	char			*str;
	char			*dirpath;
	int				flag;

	str = ft_strdup("");
	flag = 0;
	dirpath = find_dirpath(f_wc, &flag);
	str = build_str(dirpath, str, flag);
	return (str);
}

char	*get_wc_data(char *search, char *src, int mode)
{
	char			*str;
	char			*ret;
	void			*select;

	if (mode == 0)
		select = &ft_strnstr;
	else if (mode == 1)
		select = &ft_strend;
	else if (mode == 2)
		select = &ft_strnstr;
	if (!src)
	{
		str = build_wc_cd(search);
		if (!str)
			return (NULL);
		ret = find_matching(search, str, select, mode);
	}
	else
		ret = find_matching(search, src, select, mode);
	return (ret);
}

char	*wc_symbol_found(char *ret, int *i)
{
	int		start_index;
	int		end_index;
	char	*tmp;

	start_index = get_start_index(ret, *i);
	end_index = get_end_index(ret, *i);
	tmp = get_wildcard(ft_str_extract(ret + start_index,
				end_index - start_index));
	*i = (*i - (end_index - start_index) + (end_index - *i))
		+ ft_strlen(tmp);
	ret = str_replace_free(ret, start_index, end_index - start_index
			+ 1, tmp);
	return (ret);
}

char	*ft_wildcard(char *str)
{
	int		i;
	char	*ret;

	ret = ft_strdup(str);
	i = 0;
	while (ret[i])
	{
		if (ret[i] == '*')
			ret = wc_symbol_found(ret, &i);
		else
			i++;
	}
	free(str);
	return (ret);
}

void	set_wc(t_data *data)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = *data->cmd_list;
	while (cmd)
	{
		if (cmd->type == CMD)
		{
			cmd->cmd = ft_wildcard(cmd->cmd);
			i = -1;
			while (cmd->args[++i])
				cmd->args[i] = ft_wildcard(cmd->args[i]);
		}
		cmd = cmd->next;
	}
}
