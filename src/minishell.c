/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:33:14 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/20 16:53:25 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status = 0;

int	main(int argc, char *argv[], char **envp)
{
	// char	*shlvl;

	if (argc > 1)
		print_message(ARGS_ERR_MSG, RED, EXIT_FAILURE);
	(void) argv;
	if (!envp[0])
		print_message(UNAVAILABLE_ENV, RED, EXIT_FAILURE);
	print_start();
	// shlvl = ft_itoa(ft_atoi(get_env("SHLVL", &envp)) + 1, MAIN);
	// set_env("SHLVL", shlvl, &envp);
	// mem_remove(shlvl, MAIN);
	prompt_loop(&envp);
	return (EXIT_SUCCESS);
}
