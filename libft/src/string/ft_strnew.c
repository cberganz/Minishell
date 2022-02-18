/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:33:56 by cberganz          #+#    #+#             */
/*   Updated: 2021/11/24 13:33:57 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*nstr;

	nstr = (char *)malloc(size + 1 * sizeof(char));
	if (!nstr)
		return (NULL);
	ft_memset(nstr, '\0', size + 1);
	return (nstr);
}
