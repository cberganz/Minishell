/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:58:56 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/22 16:03:07 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*global_parsing(char *input)
{
	t_list	*list;

	list = parse_step1(input);
	if (!list)
		return (NULL);
	if (!single_pipe_parsing(&list))
		return (NULL);
	return (list);
}
