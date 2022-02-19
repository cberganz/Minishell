/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:03:54 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 14:15:13 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(int exit_code)
{
	del_garbage();
	rl_clear_history();//verifier si possible de recuperer historique apres execution
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
