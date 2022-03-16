/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc_variables.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/16 16:08:54 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// a mettre dans une fonction qui parcours tous les maillons d'une liste chainee et sa sous liste
// cad vraiment juste apres le parsing

// remplacement de variables dans le heredoc

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
				if (str[i] == '$' && ft_ischarset(str[i + 1], "?", ft_isalnum)
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
		if (str[i] && str[i] == '$' && ft_ischarset(str[i + 1], "?@#*-_",
				ft_isalnum) && previous_token_ismeta(str, i) != 4)
			return (1);
		if (str[i])
			i++;
	}
	return (0);
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

int	insert_var_str(char **str, int start, char ***envp, char *charset)
{
	int		stop;
	char	*to_insert;
	char	*command;

	to_insert = NULL;
	command = *str;
	stop = stop_len(command, start); 
	if (command[start + 1] == '?')
		to_insert = ft_itoa(g_status, LOOP);
	else if (ft_ischarset(command[start + 1], /*"_@#*-"*/charset, ft_isalnum))
	{
		to_insert = get_to_insert(command, start, stop, envp);// faire remonter envp
		if (!to_insert)
			return (0);
	}
	if (ft_strinsert(&command, to_insert, start, stop))
		print_message("Allocation error.\n", RED, 1);
	*str = command;
	return ((int)ft_strlen(to_insert));
}

void	redirection_var_expand(int var_expand, char **input, char ***envp, char *charset)
{
	int		i;
	int		double_quote;

	i = 0;
	if (var_expand == 1)
	{
		while (flag(&(*input)[i]))
		{
			double_quote = 0;
			while ((*input)[i])
			{
				if ((*input)[i] == '$' && ft_ischarset((*input)[i + 1], /*"?_@#*-"*/charset, ft_isalnum))
					i += insert_var_str(input, i, envp, charset);
				jump_quotes(*input, &double_quote, &i);
				i++;
			}
		}
	}
}

