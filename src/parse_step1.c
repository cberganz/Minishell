/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:24:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/20 19:03:58 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_control_op(t_list **lst, char *input)
{
	t_list	*tmp;
	int		i;

	tmp = (*lst)->next;
	i = 0;
	while (tmp)
	{
		while (input[i])
		{
			if (input[i] == '|' && input[i + 1] == '|')
			{
				((t_command *)tmp->content)->control_op = "||";
				i += 2;
				break ;
			}
			if (input[i] == '&' && input[i + 1] == '&')
			{
				((t_command *)tmp->content)->control_op = "&&";
				i += 2;
				break ;
			}
			i++;
		}
		tmp = tmp->next;
	}
}

static void iter_trim(t_list **lst)
{
	t_list	*tmp_lst;
	char	*tmp_string;

	tmp_lst = *lst;
	while (tmp_lst)
	{
		tmp_string = ft_strtrim(((t_command *)tmp_lst->content)->command, " ");
		mem_remove(((t_command *)tmp_lst->content)->command);
		((t_command *)tmp_lst->content)->command = tmp_string;
		tmp_lst = tmp_lst->next;
	}
}

static void	*parse(char *input, t_list **lst)
{
	char		**splited_input;
	t_command	*tmp;

	splited_input = ft_split_double(input, "||", "&&");
	while (*splited_input)
	{
		if (mem_alloc(sizeof(t_command), (void **)&tmp))
			return (NULL);
		tmp->command = *splited_input;
		tmp->control_op = NULL;
		ft_lstadd_back(lst, ft_lstnew(tmp));
		splited_input++;
	}
	set_control_op(lst, input);
	return ((void *)1);
}

t_list	*parse_step1(char *input)
{
	t_list		*command_list;

	command_list = NULL;
	if (!parse(input, &command_list))
		return (NULL);
	iter_trim(&command_list);
	return (command_list);
}

//int	main(void)
//{
//	t_list	*list;
//	t_list	*first;
//
//	list = parse_step1("test1 || test2 && test3 & test4 || test5 && test6 || test7");
//	first = list;
//	while (list)
//	{
//		if (((t_command *)list->content)->control_op)
//			printf("%s\n", (char *)((t_command *)list->content)->control_op);
//		else
//			printf("(null)\n");
//		printf("%s\n\n", (char *)((t_command *)list->content)->command);
//		list = list->next;
//	}
//	//ft_lstclear(&first, free);
//	//del_garbage();
//	return (0);
//}
