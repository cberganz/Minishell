/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 15:41:07 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/05 15:44:17 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size, int garbage)
{
	void	*new;

	if (!ptr)
		return (NULL);
	if (mem_alloc(new_size, &new, garbage))
	{
		mem_remove(ptr, garbage);
		return (NULL);
	}
	ft_memcpy(new, ptr, prev_size < new_size ? prev_size : new_size);
	mem_remove(ptr, garbage);
	return (new);
}
