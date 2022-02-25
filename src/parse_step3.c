/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:38:24 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/25 13:11:00 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	ft_isstatus(int c)
{
	return (c == '?');
}

static uint8_t	flag_in_str(char *str)
{
	while (*str)
	{
		if (*str == '"')
		{
			str++;
			while (*str && *str != '"')
			{
				if (*str == '$' && (ft_isalnum(*(str + 1)) || *(str + 1) == '?'))
					return (1);
				str++;
			}
		}
		if (*str && *str == 39)
		{
			str++;
			while (*str && *str != 39)
				str++;
		}
		if (*str && *str == '$' && (ft_isalnum(*(str + 1)) || *(str + 1) == '?' || *(str + 1) == 39 || *(str + 1) == '"'))
			return (1);
		if (*str)
			str++;
	}
	return (0);
}

static char	*get_to_find(char *s)
{
	int		size;
	char	*to_find;

	size = 1;
	if (!ft_isdigit(s[size]))
	{
		while (ft_isalnum(s[size]))
			size++;
	}
	else
		size = 2;
	if (mem_alloc(size, (void **)&to_find))
		return (NULL);
	ft_strlcpy(to_find, s + 1, size);
	return (to_find);
}

static uint8_t	insert(t_list *command_list, int i, int double_quote)
{
	char	*to_find;
	char	*to_insert;
	char	*command;

	command = ((t_pipe_command *)command_list->content)->cmd_content;
	if (double_quote && (command[i + 1] == 39 || command[i + 1] == '"'))
		return (0);
	if (command[i] == '$'
		&& (ft_isalnum(command[i + 1]) || command[i + 1] == '?' || command[i + 1] == 39 || command[i + 1] == '"'))
	{
		if (command[i + 1] == '?')
		{
			to_insert = ft_itoa(g_status);
			if (ft_strinsert(&((t_pipe_command *)command_list->content)->cmd_content, to_insert, i, ft_isstatus))
				print_message("Allocation error.\n", RED, 1);
		}
		else
		{
			to_find = get_to_find(&command[i]);
			if (!to_find)
				print_message("Allocation error.\n", RED, 1);
			to_insert = getenv(to_find);
			if (!to_insert)
				to_insert = "";
			mem_remove(to_find);
			if (ft_strinsert(&((t_pipe_command *)command_list->content)->cmd_content, to_insert, i, ft_isalnum))
				print_message("Allocation error.\n", RED, 1);
		}
		return (1);
	}
	return (0);
}

uint8_t	expansion_parsing(t_list *command_list)
{
	int 	i;
	uint8_t	double_quote;

	while (command_list)
	{
		while (flag_in_str(((t_pipe_command *)command_list->content)->cmd_content))
		{
			i = 0;
			double_quote = 0;
			while (((t_pipe_command *)command_list->content)->cmd_content[i])
			{
				if (!double_quote && ((t_pipe_command *)command_list->content)->cmd_content[i] == 39)
				{
					i++;
					while (((t_pipe_command *)command_list->content)->cmd_content[i] != 39)
						i++;
				}
				if (!double_quote && ((t_pipe_command *)command_list->content)->cmd_content[i] == '"')
				{
					double_quote = 1;
					i++;
				}
				if (double_quote && ((t_pipe_command *)command_list->content)->cmd_content[i] == '"')
					double_quote = 0;
				if (insert(command_list, i, double_quote))
					break ;
				i++;
			}
		}
		command_list = command_list->next;
	}
	return (0);
}

// ERREUR : $HOME | $UNFOUND -> le maillon de liste chainee n'est pas supprimer lorsque la variable n'existe pas, a gerer ici ?
// erreur : "$''"
