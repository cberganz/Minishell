/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:33:14 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/21 15:24:39 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status = 0;

int	main(int argc, char *argv[], char **envp)
{
	if (argc > 1)
		print_message(ARGS_ERR_MSG, RED, EXIT_FAILURE);
	(void) argv;
	if (!envp[0])
		print_message(UNAVAILABLE_ENV, RED, EXIT_FAILURE);
	print_start();
	handle_shlvl(1, &envp);
	prompt_loop(&envp);
	return (EXIT_SUCCESS);
}
