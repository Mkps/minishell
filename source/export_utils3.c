/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uaupetit <uaupetit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:25:55 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/22 17:02:56 by uaupetit         ###   ########.fr       */
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
	free(str);
    return (result);
}

int     export_key_exists(t_export *export, char *key_to_check)
{
    while (export != NULL)
    {
        if (ft_strncmp(export->key, key_to_check, ft_strlen(key_to_check)) == 0)
        {
            printf("key exist = 1");
            return 1;
        }
        export = export->next;
    }
    printf("key exist = 0");
    return 0;
}

int key_is_valid(char *str)
{
    int i = 0;
    
    if (str[i] == '\0')
        return 1;
    if (!ft_isalpha(str[i]) && str[i] != '_')
        return 1;
    while (str[i] != '\0')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 1;
        i++;
    }
    return 0;
}
