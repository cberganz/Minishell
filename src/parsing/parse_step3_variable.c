/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step3_variable.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:38:24 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/16 16:42:58 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
**	Argument mode set to 1 :
**	Return 3 for ">>", 2 for ">", 1 for "<", 4 for "<<" or  0 any other case
*/

static uint8_t	previous_token_ismeta(char *command, int i)
{
	while (i > 0 && command[i] != ' ' && command[i] != '\t'
			&& command[i] != '>' && command[i] != '<')
		i--;
	while (i > 0 && (command[i] == ' ' || command[i] == '\t'))
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
			{
				return (4);
			}
			return (3);
		}
	}
	return (0);
}

static uint8_t	flag(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$' && ft_ischarset(str[i + 1], "?@#*-_", ft_isalnum)
					&& previous_token_ismeta(str, i) != 4)
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
		if (str[i] && str[i] == '$' && ft_ischarset(str[i + 1], "?@#*-_\'\"",
				ft_isalnum) && previous_token_ismeta(str, i) != 4)
			return (1);
		if (str[i])
			i++;
	}
	return (0);
}

char	*get_to_insert(char *s, int pos, int size, char **envp[])
{
	char	*to_find;
	char	*to_insert;

	if (mem_alloc(size, (void **)&to_find, LOOP))
		print_message("Allocation error.\n", RED, 1);
	ft_strlcpy(to_find, s + pos + 1, size);
	to_insert = get_env(to_find, envp);
	mem_remove(to_find, LOOP);
//	if (!to_insert) // to remove ?
//		to_insert = "";
	if (previous_token_ismeta(s, pos) == 4)
		return (NULL);
	else if (previous_token_ismeta(s, pos) != 0)
		to_insert = ft_stradd_quotes(to_insert, LOOP);
	if (to_insert == NULL)
		print_message("Minishell: Allocation error\n", RED, 1);
	return (to_insert);
}

int	stop_len(char *s, int start)
{
	int	stop;

	stop = 1;
	if (ft_ischarset(s[start + stop], "?@#*-", ft_isdigit))
		stop++;
	else if (!ft_ischarset(s[start + stop], "\'\"", NULL))
	{
		while (s[start + stop] && ft_ischarset(s[start + stop], "_", ft_isalnum))
			stop++;
	}
	return (stop);
}

static int	insert(t_list *command_list, int start, char **envp[])
{
	int		stop;
	char	*to_insert;
	char	*command;

	to_insert = NULL;
	command = ((t_pipe_command *)command_list->content)->cmd_content;
	stop = stop_len(command, start); 
	if (command[start + 1] == '?')
		to_insert = ft_itoa(g_status, LOOP);
	else if (ft_ischarset(command[start + 1], "\'\"_@#*-", ft_isalnum))
	{
		to_insert = get_to_insert(command, start, stop, envp);
		if (!to_insert)
			return (0);
	}
	if (ft_strinsert(&command, to_insert, start, stop))
		print_message("Allocation error.\n", RED, 1);
	((t_pipe_command *)command_list->content)->cmd_content = command;
	return ((int)ft_strlen(to_insert));
}

static void	jump_quotes(char *cmd, int *double_quote, int *i)
{
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '\'')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '\'')
			*i += 1;
	}
	if (cmd[*i] && !(*double_quote) && cmd[*i] == '"')
		*double_quote = 1;
	else if (cmd[*i] && *double_quote && cmd[*i] == '"')
		*double_quote = 0;
}

void	variable_expansion(t_list *command_list, char **envp[])
{
	int		i;
	int		double_quote;
	char	*command;

	i = 0;
	while (command_list)
	{
		while (flag(&((t_pipe_command *)command_list->content)->cmd_content[i]))
		{
			double_quote = 0;
			command = ((t_pipe_command *)command_list->content)->cmd_content;
			while (command[i])
			{
				jump_quotes(command, &double_quote, &i);
				if (command[i] == '$' && ft_ischarset(command[i + 1], "?\'\"_@#*-", ft_isalnum))
					i += insert(command_list, i, envp) - 1;
				i++;
			}
		}
		command_list = command_list->next;
	}
}

// ERREUR : $HOME | $UNFOUND -> le maillon de liste chainee n'est pas supprimer lorsque la variable n'existe pas, a gerer ici
