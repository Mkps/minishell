/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 12:25:41 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/27 18:46:42 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// /** Gets the input **/ 
int	get_flag(char *limiter)
{
	int	i;
	int	quote_status;

	quote_status = 0;
	i = -1;
	while (limiter[++i])
	{
		if (limiter[i] == '\'')
			quote_status++;
	}
	return (!(quote_status % 2 == 0));
}

int	var_expand_dollar_notoken(t_data *data, char **ret, int *i)
{
	char	*tmp;
	int		flag_retokenize;

	flag_retokenize = 0;
	if (ft_isalpha(*(*ret + *i + 1)) || *(*ret + *i + 1) == '_')
		flag_retokenize = var_expand_valid(data, ret, i);
	else if (ft_isdigit(*(*ret + *i + 1)))
	{
		tmp = *ret;
		*ret = str_replace(*ret, *i, 2, "");
		free(tmp);
		*i = 0;
	}
	else if (*(*ret + *i + 1) == '?')
		var_expand_exitcode(data, ret, i);
	else
		*i += 1;
	return (flag_retokenize);
}

char	*heredoc_var_expand(t_data *data, char *str, int flag)
{
	int		i;
	char	*ret;
	char	**ret_ptr;

	i = 0;
	if (flag)
		return (str);
	ret_ptr = malloc(sizeof(char **));
	*ret_ptr = ft_strdup(str);
	while (*(*ret_ptr + i))
	{
		if (*(*ret_ptr + i) == '$')
			var_expand_dollar_notoken(data, ret_ptr, &i);
		else
			i++;
	}
	free(str);
	ret = ft_strdup(*ret_ptr);
	free(*ret_ptr);
	free(ret_ptr);
	return (ret);
}
