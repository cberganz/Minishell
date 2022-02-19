/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:04:29 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 12:48:46 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(void)
{
	char	*prompt;
	char	*home;

	home = getenv("HOME");
	prompt = getcwd(NULL, 0);
	if (!prompt)
		ft_exit(1); // exit_code a choisir
	garbage_addptr(prompt);
	if (home && ft_strstr(prompt, home))
	{
		prompt[ft_strlen(home) - 1] = '~';
		prompt = &prompt[ft_strlen(home) - 1];
	}
	if (g_status)
		prompt = ft_strjoin("\x1b[31;1m➜ \x1b[36m", prompt);
	else
		prompt = ft_strjoin("\x1b[32;1m➜ \x1b[36m", prompt);
	prompt = ft_strjoin(prompt, "$ \x1b[0m");
	if (!prompt)
		ft_exit(1); // exit_code a choisir
	return (prompt);
}

void	prompt(void)
{
	char	*shell_prompt;
	char	*input;

	del_garbage();
	shell_prompt = create_prompt();
	input = readline(shell_prompt);
	if (!input)
	{
		printf("exit\n");
		ft_exit(g_status);
	}
	garbage_addptr(input);
	if (!ft_strequ(input, ""))
		g_status = 0;
	printf(RED "%s\n" RESET, input);
	if (!g_status && !ft_strequ(input, ""))
	{
		g_status = 0;
		add_history(input);
	}
}
