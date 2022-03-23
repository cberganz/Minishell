/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_removefd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 14:36:15 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/23 14:38:48 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	clean_first_el(t_list *tmp, void *ptr_to_free, int id)
{
	t_list	**alst;

	if (tmp->content == ptr_to_free)
	{
		alst = garbage(id);
		*alst = tmp->next;
		close((*(int *)tmp->content));
		free(tmp);
		return (1);
	}
	return (0);
}

void	mem_remove(void *ptr_to_free, int id)
{
	t_list	*tmp;
	t_list	*prev_el;

	if (*garbage(id) == NULL)
		return ;
	tmp = *garbage(id);
	if (clean_first_el(tmp, ptr_to_free, id))
		return ;
	while (tmp && tmp->content != ptr_to_free)
	{
		prev_el = tmp;
		tmp = tmp->next;
	}
	if (tmp && tmp->content == ptr_to_free)
	{
		prev_el->next = tmp->next;
		close((*(int *)tmp->content));
		free(tmp);
	}
}
