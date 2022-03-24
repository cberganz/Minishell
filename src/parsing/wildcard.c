/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:30:10 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/24 13:17:19 by cberganz         ###   ########.fr       */
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
		if (narg)
		{
			narg = ft_sort_strarr(narg, LOOP);
			if (!narg)
				print_message("minishell: Allocation error\n", RED, 2);
		}
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

//static char	*get_match(char *arg, char *file, int arg_i, int file_i)
//{
//	while (arg[arg_i] && file[file_i])
//	{
//		while (file[file_i] && arg[arg_i] != '*')
//		{
//			if (arg[arg_i] != file[file_i] && arg[arg_i] != '?')
//				return ("");
//			file_i++;
//			arg_i++;
//		}
//		while (arg[arg_i] == '*')
//			arg_i++;
//		if (arg[arg_i - 1] == '*' && arg[arg_i])
//		{
//			while (file[file_i] && !ft_strnequ(&file[file_i], &arg[arg_i],
//					ft_strlento(&arg[arg_i], '*') || (!ft_strnequ(&file[file_i],
//							&arg[arg_i], ft_strlento(&arg[arg_i], '*') + 1)
//						&& !arg[arg_i + 1])))
//				file_i++;
//			if (!file[file_i])
//				return ("");
//			file_i += ft_strlento(&arg[arg_i], '*');
//			arg_i += ft_strlento(&arg[arg_i], '*');
//		}
//		else if (!arg[arg_i])
//			return (ft_strjoin(file, " ", LOOP));
//	}
//	if (!arg[arg_i] && !file[file_i])
//		return (ft_strjoin(file, " ", LOOP));
//	return ("");
//}
//
//static char	*get_dir_infos(char *arg)
//{
//	DIR				*dir;
//	struct dirent	*s_dir;
//	char			*narg;
//	char			*tmp;
//
//	if (!is_wildcard(arg))
//		return (NULL);
//	narg = "";
//	tmp = garbage_addptr(getcwd(NULL, 0), LOOP);
//	dir = opendir(tmp);
//	if (dir)
//	{
//		s_dir = readdir(dir);
//		while (s_dir)
//		{
//			if (s_dir->d_name[0] != '.')
//			{
//				tmp = get_match(arg, s_dir->d_name, 0, 0);
//				narg = ft_strjoin(narg, tmp, LOOP);
//			}
//			if (!narg || !tmp)
//				print_message("minishell: Allocation error\n", RED, 2);
//			s_dir = readdir(dir);
//		}
//		closedir(dir);
//		if (narg[0] == '\0')
//			return (arg);
//		if (narg)
//		{
//			narg = ft_sort_strarr(narg, LOOP);
//			if (!narg)
//				print_message("minishell: Allocation error\n", RED, 2);
//		}
//		return (narg);
//	}
//	return (NULL);
//}
//
//void	wildcard(t_list *command_list)
//{
//	t_pipe_command	*command;
//	char			*to_insert;
//	int				argc;
//
//	while (command_list)
//	{
//		argc = 0;
//		command = (t_pipe_command *)command_list->content;
//		while (command->exec_args[argc])
//		{
//			to_insert = get_dir_infos(command->exec_args[argc]);
//			if (to_insert)
//				command->exec_args[argc] = to_insert;
//			argc++;
//		}
//		command_list = command_list->next;
//	}
//}
