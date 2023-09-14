/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:19:35 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/14 17:05:07 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_wcnode{
	struct s_wcnode	*next;
	struct s_wcnode	*prev;
	int				level;
	char			**refs;
}	t_wcnode;

int	get_start_index(char *str, int i)
{
	while (i > 0 && !ft_is_space(str[i]))
		i--;
	return (i);
}

int	get_end_index(char *str, int i)
{
	while (str[i] && !ft_is_space(str[i]))
		i++;
	return (i);
}
t_wcnode	get_last_node(t_wcnode **root)
{
	t_wcnode	tmp;

	tmp = *root;
	while (tmp)
		tmp = tmp->next;
	return (tmp);
}

void	add_node(t_wcnode **root)
{
	t_wcnode	*last;

	last = get_last_node(root);
	if (*root = NULL)
		root = malloc(sizeof (t_wcnode));
	else
		get_last_node(root)->next = malloc(sizeof (t_wcnode));
	get_last_node(root)->next = NULL;
}
char	*wc_expand(t_wcnode **root)
{
	t_wcnode	current;

	current = *root;
	
}

char	*get_wildcard(char *str)
{
	int	i;
	int	start_index;
	int	end_index;
	t_wcnode	**root;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
		{
			start_index = get_start_index(str, i);
			end_index = get_end_index(str, i);
		}
		i++;
	}
	root = ft_calloc(1, sizeof(*t_wcnode));
	i = start_index;
	while (i <= end_index)
	{
		if (str[i] == '*')
			add_node(root);
		i++;
	}
	return (wc_expand(root));
}

//char	*ft_wildcard(char *str)
//{
//
//}
int	main(void)
{
	return (0);
}
