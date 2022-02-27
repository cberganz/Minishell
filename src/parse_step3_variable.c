/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step3_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:38:24 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/27 02:40:40 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
**	Return 3 for ">>", 2 for ">", 1 for "<", 4 for "<<" or  0 any other case
*/

static uint8_t	previous_token_ismeta(char *command, int i)
{
	while (i >= 0 && command[i] != ' ' && command[i] != '\t'
			&& command[i] != '>' && command[i] != '<')
		i--;
	while (i >= 0 && (command[i] == ' ' || command[i] == '\t'))
		i--;
	if (i >= 0 && ft_ischarset(command[i], "<>", NULL))
	{
		if (command[i] == '>')
		{
			if (i > 0 && command[i - 1] == '>')
				return (2);
			return (1);
		}
		if (command[i] == '<')
		{
			if (i > 0 && command[i - 1] == '<')
				return (4);
			return (3);
		}
	}
	return (0);
}

static uint8_t	flag(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$' && ft_ischarset(str[i + 1], "?", ft_isalnum)
					&& !previous_token_ismeta(str, i))
					return (1);
				i++;
			}
		}
		if (str[i] && str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
		}
		if (str[i] && str[i] == '$' && ft_ischarset(str[i + 1], "?\'\"_",
				ft_isalnum) && previous_token_ismeta(str, i))
			return (1);
		if (str[i])
			i++;
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
		print_message("Allocation error.\n", RED, 1);
	ft_strlcpy(to_find, s + 1, size);
	return (to_find);
}

static void	insert(t_list *command_list, int i)
{
	char	*to_find;
	char	*to_insert;
	char	*command;

	command = ((t_pipe_command *)command_list->content)->cmd_content;
	if (command[i + 1] == '?')
	{
		to_insert = ft_itoa(g_status);
		if (ft_strinsert(&command, to_insert, i, "?", NULL))
			print_message("Allocation error.\n", RED, 1);
	}
	else if (ft_ischarset(command[i + 1], "\'\"_", ft_isalnum))
	{
		to_find = get_to_find(&command[i]);
		to_insert = getenv(to_find);
		if (previous_token_ismeta(command, i) == 4)
			return ;
		else if (previous_token_ismeta(command, i))
			to_insert = ft_stradd_quotes(to_insert);
		if (!to_insert)
			to_insert = "";
		mem_remove(to_find);
		if (ft_strinsert(&command, to_insert, i, "_", ft_isalnum))
			print_message("Allocation error.\n", RED, 1);
	}
	((t_pipe_command *)command_list->content)->cmd_content = command;
}

static void	jump_quotes(char *cmd, uint8_t *double_quote, int *i)
{
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '\'')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '\'')
			*i += 1;
	}
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '"')
	{
		*double_quote = 1;
		*i += 1;
	}
	if (cmd[*i] && *double_quote && cmd[*i] == '"')
		*double_quote = 0;
}

void	variable_expansion(t_list *command_list)
{
	int		i;
	uint8_t	double_quote;
	char	*command;

	while (command_list)
	{
		while (flag(((t_pipe_command *)command_list->content)->cmd_content))
		{
			i = -1;
			double_quote = 0;
			command = ((t_pipe_command *)command_list->content)->cmd_content;
			while (command[++i])
			{
				jump_quotes(command, &double_quote, &i);
				if (command[i] == '$' && (!double_quote
						|| !ft_ischarset(command[i + 1], "\'\"", NULL)))
				{
					insert(command_list, i);
					break ;
				}
			}
		}
		command_list = command_list->next;
	}
}

// ERREUR : $HOME | $UNFOUND -> le maillon de liste chainee n'est pas supprimer lorsque la variable n'existe pas, a gerer ici
