/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:23 by aloubier          #+#    #+#             */
/*   Updated: 2023/10/03 17:55:34 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//Returns the value of the var. NULL if not set.
char	*get_var(t_data *data, char *str)
{
	char	*env;

	env = ft_getenv(data->envv, str);
	if (env == NULL)
	{
		free(env);
		return (NULL);
	}
	return (env);
}

int	is_valid_var(char *str)
{
	int	i;

	if (*str == 0 || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		i++;
	}
	return (1);
}

int	var_is_multiple(char *var)
{
	int	i;

	i = -1;
	if (!var)
		return (0);
	while (var[++i])
		if (var[i] == ' ')
			return (1);
	return (0);
}

int	retokenize(t_data *data, char *str, t_token *token)
{
	char	**split;
	t_token	*current;
	int		i;

	(void)data;
	current = token;
	split = ft_split(str, ' ');
	if (!split)
		return (EXIT_FAILURE);
	token->value = ft_strdup(split[0]);
	i = 1;
	while (split[i])
	{
		insert_token_next(current, WORD, ft_strdup(split[i]));
		current->next->near_quote = current->near_quote;
		current->near_quote = 0;
		current = current->next;
		i++;
	}
	free(str);
	ft_free_tab(split);
	return (EXIT_SUCCESS);
}
