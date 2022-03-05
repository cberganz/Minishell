/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 02:34:46 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/05 20:07:51 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_directory(char *path, char **envp[])
{
	char	*cwd;
	char	buff[4097];

	getcwd(buff, 4096);
	cwd = buff;
	if (!chdir(path))
	{
		set_env("OLDPWD", cwd, envp);
		if (ft_strequ(path, get_env("HOME", envp)))
			cwd = get_env("HOME", envp);
		else
		{
			getcwd(buff, 4096);
			cwd = buff;
		}
		set_env("PWD", cwd, envp);
	}
	else
		print_dirpath_err(path);
}

int	builtin_cd(char **exec_args, int exit, char **envp[])
{
	char	*path;

	if (exec_args[0] && exec_args[1])
	{
		ft_putendl_fd("minishell: cd: too many arguments.", 2);
		if (exit)
			free_and_exit(1);
		return (1);
	}
	else if (!exec_args[0])
		path = get_env("HOME", envp);
	else
		path = exec_args[0];
	if (!path)
		print_message("cd: Allocation error.\n", RED, 1);
	change_directory(path, envp);
	if (exit)
		free_and_exit(0);
	return (0);
}
