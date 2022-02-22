/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:05:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/22 19:21:32 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
//		if (g_status == -10)
//			printf("%s", create_prompt());
//		else
//		{
		rl_on_new_line();
		rl_redisplay(); // Probleme ne met pas la fleche en rouge
//		}
		g_status = 130;
	}
	if (sigcode == SIGQUIT || sigcode == SIGTSTP)
		printf("\b\b  \033[2D");
}
