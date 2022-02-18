/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 15:40:55 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/18 13:57:36 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage.h"

void	del_garbage(t_list *lst)
{
	free(lst->content);
	free(lst);
}

void	mem_alloc(t_garbage *garbage, void **ptr, int size)
{
	t_list	*el;

	*ptr = malloc(size);
	if (ptr == NULL)
	{
		ft_lstclear(&garbage->collector, del_garbage);
		exit (EXIT_FAILURE);
	}
	el = ft_lstnew(*ptr);
	if (el == NULL)
	{
		free(*ptr);
		ft_lstclear(&garbage->collector, del_garbage);
		exit (EXIT_FAILURE);
	}
	if (garbage->collector == NULL)
		garbage->collector = el;
	else
		ft_lstadd_front(&garbage->collector, el);
}

static int	clean_first_el(t_list *tmp, void *ptr_to_free, t_garbage *garbage)
{
	if (tmp->content == ptr_to_free)
	{
		if (tmp->next == NULL)
		{
			free(tmp->content);
			free(tmp);
			garbage->collector = NULL;
		}
		else
		{
			garbage->collector = tmp->next;
			free(tmp->content);
			free(tmp);
		}
		return (1);
	}
	return (0);
}

void	mem_remove(t_garbage *garbage, void *ptr_to_free)
{
	t_list	*tmp;
	t_list	*prev_el;

	if (garbage->collector == NULL)
		return ;
	tmp = garbage->collector;
	if (clean_first_el(tmp, ptr_to_free, garbage))
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
