/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:30:10 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/05 18:58:19 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	is_wildcard(char *arg)
{
	while (*arg)
	{
		if (*arg == '"')
			arg += quote_len(&(*arg), '"') + 1;
		else if (*arg == '\'')
			arg += quote_len(&(*arg), '\'') + 1;
		else if (*arg == '*' || *arg == '?')
			return (1);
		arg++;
	}
	return (0);
}

static char	*get_match(char *arg, char *file)
{
	int		arg_i;
	int		file_i;

	arg_i = 0;
	file_i = 0;
	while (arg[arg_i] && file[file_i])
	{
		while (file[file_i] && arg[arg_i] != '*') 
		{
		    if (arg[arg_i] != file[file_i] && arg[arg_i] != '?')
			{
		        return ("");
			}
			file_i++;
			arg_i++;
		}
		while (arg[arg_i] == '*')
			arg_i++;
		if (arg[arg_i - 1] == '*' && arg[arg_i]) 
		{
			while (file[file_i]
				&& (!ft_strnequ(&file[file_i], &arg[arg_i], ft_strlento(&arg[arg_i], '*'))
				|| (!ft_strnequ(&file[file_i], &arg[arg_i], ft_strlento(&arg[arg_i], '*') + 1) && !arg[arg_i + 1])))
				file_i++;
			if (!file[file_i])
				return ("");
			file_i += ft_strlento(&arg[arg_i], '*');
			arg_i += ft_strlento(&arg[arg_i], '*');
		}
		else if (!arg[arg_i])
			return (ft_strjoin(file, " ", LOOP));
	}
	if (!arg[arg_i] && !file[file_i])
		return (ft_strjoin(file, " ", LOOP));
	return ("");
}

static char	*get_dir_infos(char *arg)
{
	DIR				*dir;
	struct dirent	*directory;
	char			*narg;

	if (!is_wildcard(arg))
		return (NULL);
	narg = "";
	dir = opendir(getcwd(NULL, 0));
	if (dir)
	{
		directory = readdir(dir);
		while (directory)
		{
			if (directory->d_name[0] != '.')
				narg = ft_strjoin(narg, get_match(arg, directory->d_name), LOOP);
			directory = readdir(dir);
		}
		closedir(dir);
		if (narg[0] == '\0')
			return (arg);
		return (narg);
	}
	return (NULL);
}

void	wildcard(t_list *command_list)
{
	t_pipe_command	*command;
	char			*to_insert;
	int				argc;

	while (command_list)
	{
		argc = 0;
		command = (t_pipe_command *)command_list->content;
		while (command->exec_args[argc])
		{
			to_insert = get_dir_infos(command->exec_args[argc]);
			if (to_insert)
			{
			//	mem_remove(command->exec_args[argc], LOOP);
				command->exec_args[argc] = to_insert;
			}
			argc++;
		}
		command_list = command_list->next;
	}
}
