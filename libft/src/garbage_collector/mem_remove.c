/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 14:14:47 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/20 17:38:00 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	clean_first_el(t_list *tmp, void *ptr_to_free)
{
	t_list	**alst;

	if (tmp->content == ptr_to_free)
	{
		alst = garbage();
		*alst = tmp->next;
		free(tmp->content);
		free(tmp);
		return (1);
	}
	return (0);
}

void	mem_remove(void *ptr_to_free)
{
	t_list	*tmp;
	t_list	*prev_el;

	if (*garbage() == NULL)
		return ;
	tmp = *garbage();
	if (clean_first_el(tmp, ptr_to_free))
		return ;
	while (tmp && tmp->content != ptr_to_free)
	{
		prev_el = tmp;
		tmp = tmp->next;
	}
	if (tmp && tmp->content == ptr_to_free)
	{
		prev_el->next = tmp->next;
		free(tmp->content);
		free(tmp);
	}
}
