/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:24:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/20 15:46:58 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_control_op(t_list **lst, char *operator)
{
	t_list	*tmp;

	tmp = *lst;
	tmp = tmp->next;
	while (tmp)
	{
		if (((t_command *)tmp->content)->control_op == NULL)
			((t_command *)tmp->content)->control_op = operator;
		tmp = tmp->next;
	}
}

static void	*parse_doubleand(t_list **lst)
{
	t_command	*tmp;
	t_list		*tmp_lst;
	t_list		*previous;
	char		**tmp_char;
	int			i;

	tmp_lst = *lst;
	while (tmp_lst)
	{
		i = 1;
		if (ft_strstr(*((t_command *)tmp_lst->content)->command, "&&"))
			tmp_char = ft_split(*((t_command *)tmp_lst->content)->command, "&&");
		else
		{
			previous = tmp_lst;
			tmp_lst = tmp_lst->next;
			continue ;
		}
		while (*tmp_char)
		{
			if (mem_alloc(sizeof(t_command), (void **)&tmp))
				return (NULL);
			tmp->command = &(*tmp_char);
			if (i == 1)
				tmp->control_op = "||";
			else	
				tmp->control_op = NULL;
			ft_lstadd_here(previous->next, ft_lstnew(tmp));	
			previous = tmp_lst;
			tmp_lst = tmp_lst->next;
			tmp_char++;
			i++;
		}
		previous = tmp_lst;
		tmp_lst = tmp_lst->next;
	}
	set_control_op(lst, "&&");
	return ((void *)1);
}

static void	*parse_doublepipe(char *input, t_list **lst)
{
	char		**splited_input;
	t_command	*tmp;

	splited_input = ft_split(input, "||");
	while (*splited_input)
	{
		if (mem_alloc(sizeof(t_command), (void **)&tmp))
			return (NULL);
		tmp->command = splited_input;
		tmp->control_op = NULL;
		ft_lstadd_back(lst, ft_lstnew(tmp));
		splited_input++;
	}
	set_control_op(lst, "||");
	return ((void *)1);
}

t_list	*parse_step1(char *input)
{
	t_list		*command_list;

	command_list = NULL;
	if (!parse_doublepipe(input, &command_list))
		return (NULL);
	if (!parse_doubleand(&command_list))
		return (NULL);
	return (command_list);
}

int	main(void)
{
	t_list	*list;
	t_list	*first;

	list = parse_step1("test1 || test2 && test3 & test4 || test5");
	first = list;
	while (list)
	{
		if (((t_command *)list->content)->control_op)
			printf("%s\n", (char *)((t_command *)list->content)->control_op);
		else
			printf("(null)\n");
		printf("%s\n\n", (char *)*((t_command *)list->content)->command);
		list = list->next;
	}
	ft_lstclear(&first, free);
	return (0);
}

//char	**parse_pipes()
//{
//
//}
