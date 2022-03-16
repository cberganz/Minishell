/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:05:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/16 11:36:43 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		printf("\n");
		if (g_status == -255)
			close(STDIN_FILENO);
		else if (g_status == -256)
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay(); // Probleme ne met pas la fleche en rouge => peut etre reglé avec le close STDIN
		}
		g_status = 130;
	}
	if (sigcode == SIGQUIT || sigcode == SIGTSTP)
		printf("\b\b  \033[2D"); 
}
