/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 14:13:57 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/18 17:07:57 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int8_t	mem_alloc(unsigned long size, void **ptr)
{
	t_list	*el;

	*ptr = malloc(size);
	if (*ptr == NULL)
		return (-1);
	el = garbage_lstnew(*ptr);
	if (el == NULL)
	{
		free(*ptr);
		return (-1);
	}
	ft_lstadd_front(garbage(), el);
	return (0);
}
