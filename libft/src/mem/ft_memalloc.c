/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:28:40 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/18 16:40:24 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*alloc;

	if (size == 0)
		return (NULL);
	if (mem_alloc(size * sizeof(char), &alloc))
		return (NULL);
	ft_bzero(alloc, size);
	return (alloc);
}
