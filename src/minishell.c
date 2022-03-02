/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:33:14 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/02 04:03:20 by charles          ###   ########.fr       */
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
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	prompt_loop(envp);
	return (EXIT_SUCCESS);
}
