/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:30:10 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/11 05:35:06 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	is_wildcard(char *arg)
{
	while (*arg)
	{
		if (*arg == '"')
			arg += quote_len(&(*arg), '"');
		else if (*arg == '\'')
			arg += quote_len(&(*arg), '\'');
		else if (*arg == '*' || *arg == '?')
			return (1);
		arg++;
	}
	return (0);
}

static char	*sort_str(char *str)
{
	int		i;
	int		j;
	char	*tmp;
	char	**split;
	char	**lower;

	split = ft_split(str, " ", LOOP);
	lower = malloc((ft_strarr_size(split) + 1) * sizeof(char *));
	if (!split || !lower)
		print_message("minishell: Allocation error\n", RED, 1);
	i = -1;
	while (split[++i])
		lower[i] = ft_strmap(split[i], ft_tolower, LOOP);
	i = -1;
	while (split[++i])
	{
		j = i;
		while (split[++j])
		{
			if (ft_strcmp(lower[i], lower[j]) > 0)
			{
				tmp = split[i];
				split[i] = split[j];
				split[j] = tmp;
				tmp = lower[i];
				lower[i] = lower[j];
				lower[j] = tmp;
			}
		}
	}
	i = -1;
	tmp = "";
	while (split[++i] && split[i + 1])
	{
		tmp = ft_strjoin(tmp, split[i], LOOP);
		tmp = ft_strjoin(tmp, " ", LOOP);
	}
	tmp = ft_strjoin(tmp, split[i], LOOP);
	return (tmp);
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
	char			*cwd;

	if (!is_wildcard(arg))
		return (NULL);
	narg = "";
	cwd = getcwd(NULL, 0);
	dir = opendir(cwd);
	free(cwd);
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
		return (sort_str(narg));
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
