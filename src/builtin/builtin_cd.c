/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 02:34:46 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/22 10:44:25 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_path(char *cwd, char *path)
{
	char	*ncwd;
	int		i;
	int		nb_slash;
	uint8_t	is_path;

	nb_slash = 0;
	is_path = 1;
	while (path[nb_slash] == '/')
		nb_slash++;
	if (path[nb_slash] == '\0')
		is_path = 0;
	if (nb_slash == 2)
	{
		if (mem_alloc(ft_strlen(cwd) + 2, (void **)&ncwd, LOOP))
			print_message(MALLOC_ERR_MSG, RED, 1);
		ncwd[0] = '/';
		i = 1;
		while (*cwd)
		{
			ncwd[i] = *cwd;
			cwd++;
			i++;
		}
		ncwd[i] = '\0';
		return (ncwd);
	}
	else if (nb_slash > 2 && !is_path)
		return ("/");
	return (cwd);
}

static int	change_directory(char *path, char **envp[])
{
	char	*cwd;
	char	buff[4097];

	cwd = get_env("PWD", envp);
	if (path[0] == '.' && path[1] == '\0')
		path = ft_strjoin(cwd, "/.", LOOP); // A revoir ?
	else if (path[0] == '.' && path[1] == '.' && path[2] == '\0')
		path = ft_strjoin(cwd, "/..", LOOP); // A revoir ?
	if (!chdir(path))
	{
		set_env("OLDPWD", cwd, envp);
		//if (ft_strequ(path, get_env("HOME", envp)))
		//	cwd = get_env("HOME", envp);
		//else
		//{
		getcwd(buff, 4096);
		cwd = check_path(buff, path);
		//}
		set_env("PWD", cwd, envp);
		return (0);
	}
	else
		return (print_dirpath_err(path));
}

int	builtin_cd(char **exec_args, char **envp[], int exit)
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
			return (1); // return 1 or free_and_exit ?
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
