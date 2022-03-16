/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:38 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/14 16:43:01 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **exec_args)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		print_message("pwd: Allocation error.\n", RED, 1);
	printf("%s\n", cwd);
	// if (exit)
	// 	free_and_exit(0);
	(void)exec_args;
	return (0);
}
