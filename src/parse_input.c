/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:24:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 19:00:27 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*parse_commands(char *input)
{
	char	**splited_input;
	t_list	*command_list;

	command_list = NULL;
	splited_input = ft_split(input, "||");
	while (*splited_input)
	{
		ft_lstadd_back(&command_list, ft_lstnew(*splited_input));
		splited_input++;
	}
	return (command_list);
}

int	main(void)
{
	t_list	*list;

	list = parse_commands("test || asjdas && sdasd & sdsd | sdsd");
	while (list)
	{
		printf("%s\n", (char *)list->content);
		list = list->next;
	}
	return (0);
}

//char	**parse_pipes()
//{
//
//}
