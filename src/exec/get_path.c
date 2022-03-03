/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 15:18:15 by charles           #+#    #+#             */
/*   Updated: 2022/03/03 10:49:09 by cberganz         ###   ########.fr       */
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

	path = ft_split(get_env("PATH", envp), ":", LOOP);
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
