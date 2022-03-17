/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:38 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/17 03:51:11 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **exec_args)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putendl_fd("pwd : erreur de détermination du répertoire actuel : getcwd : ne peut accéder aux répertoires parents : Aucun fichier ou dossier de ce type", 2);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	// if (exit)
	// 	free_and_exit(0);
	(void)exec_args;
	return (0);
}
