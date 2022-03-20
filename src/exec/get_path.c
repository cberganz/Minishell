/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:33:18 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/20 14:46:05 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		mem_remove(arr[i], LOOP);
		i++;
	}
	mem_remove(arr, LOOP);
}

char	*get_path(char **exec_args, char **envp[])
{
	int				i;
	char			*bin_path;
	char			**path;
	struct stat		stat;

	path = NULL;
	bin_path = get_env("PATH", envp);
	if (!bin_path)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(exec_args[0], 2);
		ft_putendl_fd(": No such file or directory", 2);
		free_and_exit(127);
	}
	path = ft_split(bin_path, ":", LOOP);
	i = -1;
	while (path && path[++i])
	{
		bin_path = ft_strjoin(path[i], "/", LOOP);
		bin_path = ft_strjoin(bin_path, exec_args[0], LOOP);
		if (lstat(bin_path, &stat) == -1)
			mem_remove(bin_path, LOOP);
		else
		{
			free_arr(path);
			return (bin_path);
		}
	}
	free_arr(path);
	return (NULL);
}
