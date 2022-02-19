/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_addptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:24:27 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 19:12:08 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void *garbage_addptr(void *ptr)
{
	t_list	*el;

	el = garbage_lstnew(ptr);
	if (el == NULL)
		return (NULL);
	ft_lstadd_front(garbage(), el);
	return (ptr);
}
