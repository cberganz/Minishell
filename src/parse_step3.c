/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:38:24 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/25 15:40:26 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static uint8_t	flag_in_str(char *str)
{
	while (*str)
	{
		if (*str == '"')
		{
			str++;
			while (*str && *str != '"')
			{
				if (*str == '$' && ft_ischarset(*(str + 1), "?", ft_isalnum))
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
		if (*str && *str == '$' && ft_ischarset(*(str + 1), "?\'\"_",
				ft_isalnum))
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
		while (ft_ischarset(s[size], "_", ft_isalnum))
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
	if (double_quote && ft_ischarset(command[i + 1], "\'\"", NULL))
		return (0);
	if (command[i] == '$' && ft_ischarset(command[i + 1], "?\'\"_", ft_isalnum))
	{
		if (command[i + 1] == '?')
		{
			to_insert = ft_itoa(g_status);
			if (ft_strinsert(&((t_pipe_command *)command_list->content)->cmd_content, to_insert, i, "?", NULL))
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
			if (ft_strinsert(&((t_pipe_command *)command_list->content)->cmd_content, to_insert, i, "_", ft_isalnum))
				print_message("Allocation error.\n", RED, 1);
		}
		return (1);
	}
	return (0);
}

uint8_t	expansion_parsing(t_list *command_list)
{
	int		i;
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
