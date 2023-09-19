/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:25:55 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/18 16:57:32 by uaupetit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int     ft_wordsize(char *s, char c, int pos)
{
    int wsize;

    wsize = 0;
    while (s[pos] != c && s[pos])
    {
        wsize++;
        pos++;
    }
    return (wsize);
}

void    free_tabs(char **tab)
{
    size_t i;

    i = 0;
    while (tab[i])
    {
        if (tab[i])
            free(tab[i]);
        i++;
    }
    if (tab)
        free(tab);
}

char    *add_quotes(char *str)
{
    size_t len = ft_strlen(str);
    char *result = (char *)malloc(len + 3);

    if (result)
    {
        result[0] = '"';
        ft_strlcpy(result + 1, str, len + 2);
        result[len + 1] = '"';
        result[len + 2] = '\0';
    }
    return result;
}

int     export_key_exists(t_export *export, char *key_to_check)
{
    while (export != NULL)
    {
        if (ft_strncmp(export->key, key_to_check, ft_strlen(key_to_check)) == 0)
            return 1;
        export = export->next;
    }
    return 0;
}