/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 12:10:34 by aloubier          #+#    #+#             */
/*   Updated: 2022/11/28 12:39:48 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
int	main(void)
{
	t_list 	*first_elem;
	char	first_data[] = "FOO";
	t_list	*new_elem;
	char	new_data[] = "BAR";

	first_elem = ft_lstnew(first_data);
	new_elem = ft_lstnew(new_data);

	ft_lstadd_front(&first_elem, new_elem);
	printf("%s\n",first_elem->content);
	printf("%s\n",new_elem->content);
	printf("%s\n", new_elem->next->content);
	return (0);
}
*/
