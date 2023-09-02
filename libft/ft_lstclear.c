/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 13:00:27 by aloubier          #+#    #+#             */
/*   Updated: 2022/11/28 15:35:27 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current;

	if (!del)
		return ;
	while (*lst)
	{
		del((*lst)->content);
		current = *lst;
		*lst = current->next;
		free(current);
	}
	*lst = NULL;
}
