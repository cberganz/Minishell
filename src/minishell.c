/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:33:14 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/19 12:47:16 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	if (argc > 1)
		print_message(ARGS_ERR, RED, CLOSE_PROG);
	(void) argv;
	(void) envp;
	print_start();
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while (1)
		prompt();
	return (0);
}
