/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_fdgarbage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 14:39:09 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/23 14:43:00 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_delone(t_list *lst)
{
	if (lst && del)
	{
		close((*(int *)lst->content));
		free(lst);
	}
}

static void	ft_clear(t_list **lst)
{
	t_list	*next;

	while (*lst)
	{
		next = (*lst)->next;
		ft_lstdelone(*lst);
		*lst = next;
	}
	ft_memdel((void **)lst);
}

void	del_fdgarbage(int id)
{
	ft_clear(garbage(id));
}
