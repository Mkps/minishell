#include "../../include/minishell.h"

static char	**ft_cleartab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static int	ft_count_strings(char *str, char separator)
{
	size_t	i;
	size_t	str_count;
	int		quote_status;

	i = 0;
	str_count = 0;
	while (str[i])
	{
		while (str[i] && separator == str[i])
			i++;
		if (str[i])
			str_count++;
		while (str[i] && str[i] != separator)
		{
			if (ft_get_sep_type(&str[i]) == DQUOTE
				|| ft_get_sep_type(&str[i]) == SQUOTE
				|| ft_get_sep_type(&str[i]) == O_PAR)
			{
				if (ft_get_sep_type(&str[i]) == O_PAR)
					quote_status = C_PAR;
				else
					quote_status = ft_get_sep_type(&str[i]);
				i++;
				while (str[i] && ft_get_sep_type(&str[i]) != quote_status)
					i++;
			}
			else if (str[i])
				i++;
		}
	}
	return (str_count);
}

static char	*ft_string_extract(char *src, char separator)
{
	int		i;
	char	*dest;
	int		str_len;
	int		quote_status;

	str_len = 0;
	quote_status = 0;
	while (src[str_len] && src[str_len] != separator)
	{
		if (ft_get_sep_type(&src[str_len]) == DQUOTE
			|| ft_get_sep_type(&src[str_len]) == SQUOTE
			|| ft_get_sep_type(&src[str_len]) == O_PAR)
		{
			if (ft_get_sep_type(&src[str_len]) == O_PAR)
				quote_status = C_PAR;
			else
				quote_status = ft_get_sep_type(&src[str_len]);
			str_len++;
			while (src[str_len]
				&& ft_get_sep_type(&src[str_len]) != quote_status)
				str_len++;
		}
		else if (src[str_len])
			str_len++;
	}
	if (str_len == 0)
		return (0);
	dest = malloc(sizeof(char) * (str_len + 1));
	if (!dest)
		return (0);
	i = 0;
	while (i < str_len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	**ft_cycle_str(char **tab, char *str, char c)
{
	int	i;
	int	quote_status;

	i = 0;
	quote_status = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			tab[i] = ft_string_extract((char *)str, c);
			if (tab[i] == NULL)
				return (ft_cleartab(tab));
			if (ft_count_strings(str, c) != 0)
				i++;
		}
		while (*str && *str != c)
		{
			if (ft_get_sep_type(str) == DQUOTE || ft_get_sep_type(str) == SQUOTE
				|| ft_get_sep_type(str) == O_PAR)
			{
				if (ft_get_sep_type(str) == O_PAR)
					quote_status = C_PAR;
				else
					quote_status = ft_get_sep_type(str);
				str++;
				while (*str && ft_get_sep_type(str) != quote_status)
					str++;
			}
			else if (*str)
				str++;
		}
	}
	tab[i] = NULL;
	return (tab);
}

char	**ft_split_noquote(char *str, char c)
{
	int		string_count;
	char	**tab;

	string_count = ft_count_strings(str, c);
	tab = malloc(sizeof(str) * (string_count + 1));
	if (!tab)
		return (NULL);
	return (ft_cycle_str(tab, str, c));
}
