/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:03:54 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 13:41:14 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(int exit_code)
{
	del_garbage();
	rl_clear_history();//pas sur de le supprimer
	exit(exit_code);
}

void	error(
