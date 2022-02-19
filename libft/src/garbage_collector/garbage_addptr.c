/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_addptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:24:27 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 12:26:57 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int8_t	garbage_addptr(void *ptr)
{
	t_list	*el;

	el = garbage_lstnew(ptr);
	if (el == NULL)
		return (-1);
	ft_lstadd_front(garbage(), el);
	return (0);
}
