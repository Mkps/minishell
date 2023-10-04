/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:50:43 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/04 19:00:21 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	var_expand_valid(t_data *data, char **ret, int *i)
{
	int		n;
	char	*var_id;
	char	*tmp_str;
	int		flag_retokenize;

	n = 1;
	flag_retokenize = 0;
	var_id = NULL;
	while (*(*ret + *i + n) && (ft_isalnum(*(*ret + *i + n)) || *(*ret + *i
				+ n) == '_'))
		n++;
	if (n != 1)
	{
		tmp_str = ft_str_extract((*ret + *i + 1), n - 1);
		var_id = get_var(data, tmp_str);
		if (var_is_multiple(var_id))
			flag_retokenize++;
		*ret = str_replace_strs(ret, *i, n, var_id);
		free(tmp_str);
	}
	if (var_id)
		*i += ft_strlen(var_id);
	return (flag_retokenize);
}

void	var_expand_exitcode(t_data *data, char **ret, int *i)
{
	char	*tmp;
	char	*exit_code;

	exit_code = ft_itoa(data->exit_status);
	tmp = *ret;
	*ret = str_replace(*ret, *i, 2, exit_code);
	free(tmp);
	free(exit_code);
	*i += 2;
}

int	var_expand_dollar(t_data *data, char **ret, int *i, t_token *token)
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
		*i += 2;
	}
	else if ((*(*ret + *i + 1) == 0) && token->near_quote == 1 && (!token->next
			|| (token->next && token_is_quote(token->next))))
		*(*ret + *i) = 0;
	else if (*(*ret + *i + 1) == '?')
		var_expand_exitcode(data, ret, i);
	else
		*i += 1;
	return (flag_retokenize);
}

// Replaces the $VAR with its' corresponding value stored in env if it exists.
int	var_expander(t_data *data, char *str, t_token **token)
{
	int		i;
	char	**ret_ptr;
	int		flag_retokenize;

	flag_retokenize = 0;
	i = 0;
	ret_ptr = malloc(sizeof(char **));
	*ret_ptr = ft_strdup(str);
	while (*(*ret_ptr + i))
	{
		if (*(*ret_ptr + i) == '$')
			flag_retokenize += var_expand_dollar(data, ret_ptr, &i, *token);
		else
			i++;
	}
	free(str);
	if (*token && flag_retokenize > 0 && (*token)->quote_status == NONE)
		if (retokenize(data, *ret_ptr, *token) == EXIT_SUCCESS)
			return (free_return(EXIT_SUCCESS, ret_ptr, 0, 0));
	if (*token)
		(*token)->value = ft_strdup(*ret_ptr);
	multi_free(*ret_ptr, ret_ptr, NULL, NULL);
	return (flag_retokenize != 0);
}
