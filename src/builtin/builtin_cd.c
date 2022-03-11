/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 02:34:46 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/11 06:17:33 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change_directory(char *path, char **envp[])
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
		return (0);
	}
	else
		return (print_dirpath_err(path));
}

int	builtin_cd(char **exec_args, int exit, char **envp[])
{
	char	*path;
	int		ret;

	ret = 0;
	if (exec_args[0] && exec_args[1])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		if (exit)
			free_and_exit(1);
		return (1);
	}
	else if (!exec_args[0])
	{
		path = get_env("HOME", envp);
		if (!path[0])
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
	}
	else
		path = exec_args[0];
	if (!path)
		print_message("cd: Allocation error\n", RED, 1);
	ret = change_directory(path, envp);
	if (exit)
		free_and_exit(ret);
	return (ret);
}
