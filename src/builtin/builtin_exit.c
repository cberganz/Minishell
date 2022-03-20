/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:22 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/20 11:34:55 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Additional tests to do for this function
uint8_t	is_numeric(char *s)
{
	int		i;
	char	*ll_min;
	char	*ll_max;
	int		over;
	int		under;
	int		ll;

	i = ft_strlen(s) - 1;
	over = 0;
	under = 0;
	ll = 18;
	ll_min = "9223372036854775808";
	ll_max = "9223372036854775807";
	while (i >= 0 && ll >= 0)
	{
		if (!ft_isdigit(s[i]))
		{
			if (s[i] == '-' && i == 0)
				break ;
			else
				return (0);
		}
		if (s[i] > ll_max[ll] || (over == 1 && s[i] == ll_max[ll]))
			over = 1;
		else
			over = 0;
		if (s[i] > ll_min[ll] || (under == 1 && s[i] == ll_min[ll]))
			under = 1;
		else
			under = 0;
		i--;
		ll--;
	}
	if ((ll < 0 && i >= 0 && i == 0 && (s[i] != '-' || under)) || (ll < 0 && i != 0 && (over || under)))
		return (0);
	return (1);
}

static uint8_t	too_many_sign(char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s == '+')
			count++;
		s++;
	}
	if (count > 1)
		return (1);
	else
		return (0);
}

int	builtin_exit(char **exec_args, int exit)
{
	int	exit_status;

	exit_status = g_status;
	if (!exit)
		ft_putendl_fd("exit", 2);
	if (exec_args[0])
	{
		if (too_many_sign(exec_args[0]) || !is_numeric(ft_strtrim(exec_args[0], " +", LOOP)))
		{
			ft_putstr_fd("Minishell: exit: ", 2);
			ft_putstr_fd(exec_args[0], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit_status = 2;
		}
		else if (exec_args[1])
		{
			ft_putendl_fd("Minishell: exit: too many arguments", 2);
			return (1);
		}
		else
			exit_status = ft_atoi(exec_args[0]);
	}
	free_and_exit(exit_status);
	return (0);
}
