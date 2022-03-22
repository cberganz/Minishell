/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:03:54 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/22 16:20:46 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(int exit_code)
{
	rl_clear_history();
	del_garbage(LOOP);
	del_garbage(MAIN);
	exit(exit_code);
}

void	child_error_exit(int status_error, char *file)
{
	printf("Minishell: ");
	if (status_error == PATH_FILE_ERR || status_error == RIGHT_FILE_ERR
		|| status_error == EXEC_ERR || status_error == FD_ERR)
	{
		if (file)
			printf("%s", file);
		perror("");
	}
	else if (status_error == PATH_CMD_ERR)
		printf("%s: Command not found.\n", file);
	else if (status_error == MALLOC_ERR)
		printf("Allocation error.\n");
	free_and_exit(status_error);
}

void	eof_exit(void)
{
	ft_putendl_fd("exit", 2);
	free_and_exit(g_status);
}
