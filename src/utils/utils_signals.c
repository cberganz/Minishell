/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:05:31 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/22 23:07:14 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		close(STDIN_FILENO);
		g_status = 130;
	}
	if (sigcode == SIGQUIT || sigcode == SIGTSTP)
		write(2, "\b\b  \033[2D", 8); 
}

void	sig_void(int sigcode)
{
	(void)sigcode;
}

void	child_sig_handler(int sigcode)
{
	if (sigcode == SIGQUIT)
	{
		ft_putendl_fd("Quit (core dumped)", 2);
		g_status = 131;	
	}
}
