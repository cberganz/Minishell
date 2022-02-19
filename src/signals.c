/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:05:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 12:17:43 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		//quand ctr-c devrait redisplay le prompt avec fleche rouge
		g_status = 130;
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (sigcode == SIGQUIT)
		printf("\b\b  \033[2D");
}
