/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:05:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 19:21:45 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		g_status = 130;
		printf("\n");
		rl_replace_line("", 0);
		printf("%s", create_prompt());//check si printf ne cree pas de pb => en cree quand pipe ouvert + ctrl-c + navigation historique
		// rl_on_new_line();
		// rl_redisplay();
	}
	if (sigcode == SIGQUIT)
		printf("\b\b  \033[2D");
}
