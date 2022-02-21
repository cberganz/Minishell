/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:28:01 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/20 19:38:54 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*nlst;

	if (mem_alloc(sizeof(*nlst), (void **)&nlst))
		return (NULL);
	nlst->content = content;
	nlst->next = NULL;
	return (nlst);
}
