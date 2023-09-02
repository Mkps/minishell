/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 12:55:16 by aloubier          #+#    #+#             */
/*   Updated: 2022/11/28 12:59:10 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst)
		return ;
	if (del)
		del(lst->content);
	free(lst);
}
/*
int	main(void)
{
	char	*str;
	t_list	*elem;

	str = ft_strdup("test");
	elem =  ft_lstnew(str);
	printf(">>content = %s", elem->content);
	ft_lstdelone(elem, free); 
	elem = 0;
	write(1, "\nTest done!\n", 13);
	return (0);
}
*/
