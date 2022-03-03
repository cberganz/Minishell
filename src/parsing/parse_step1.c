/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 18:24:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/03 10:42:58 by cberganz         ###   ########.fr       */
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
			if (input[i] == '"')
				i += quote_len(&input[i], '"');
			else if (input[i] == '\'')
				i += quote_len(&input[i], '\'');
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

static void	iter_trim(t_list **lst)
{
	t_list	*tmp_lst;
	char	*tmp_string;

	tmp_lst = *lst;
	while (tmp_lst)
	{
		tmp_string = ft_strtrim(((t_command *)tmp_lst->content)->command, " ", LOOP);
		mem_remove(((t_command *)tmp_lst->content)->command, LOOP);
		((t_command *)tmp_lst->content)->command = tmp_string;
		tmp_lst = tmp_lst->next;
	}
}

static void	*parse(char *input, t_list **lst)
{
	char		**splited_input;
	t_command	*tmp;

	splited_input = ft_split_double(input, "||", "&&", LOOP);
	while (*splited_input)
	{
		if (mem_alloc(sizeof(t_command), (void **)&tmp, LOOP))
			print_message("Allocation error.\n", RED, MALLOC_ERR);
		tmp->command = *splited_input;
		tmp->control_op = NULL;
		ft_lstadd_back(lst, ft_lstnew(tmp, LOOP));
		splited_input++;
	}
	set_control_op(lst, input);
	return ((void *)1);
}

t_list	*parse_step1(char *input)
{
	t_list		*command_list;

	if (!*input)
		return (NULL);
	command_list = NULL;
	parse(input, &command_list);
	iter_trim(&command_list);
	return (command_list);
}
