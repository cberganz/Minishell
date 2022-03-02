/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 02:00:10 by charles           #+#    #+#             */
/*   Updated: 2022/03/02 02:10:15 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **exec_args, int exit)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		print_message("pwd: Allocation error.\n", RED, 1);
	printf("%s\n", cwd);
	if (exit)
		free_and_exit(0);
	(void)exec_args;
	return(0);
}
