/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:04:29 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 19:09:51 by rbicanic         ###   ########.fr       */
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
		free_and_exit(MALLOC_ERR);
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
		free_and_exit(MALLOC_ERR);
	return (prompt);
}

uint8_t	pipe_is_open(char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	while (str[i] == ' ' && i >= 0)// check si sur ' ' ou all spaces like \t
		i--;
	if (str[i] == '|' || (str[i] == '&' && (i > 0 && str[i - 1] == '&')))
		return (1);
	return (0);
}

void	prompt(void)
{
	char	*shell_prompt;
	char	*input;

	shell_prompt = create_prompt();
	input = "";
	while (1)
	{
		// input = readline(shell_prompt);
		input = ft_strjoin(input, readline(shell_prompt));
		if (!input)
		{
			printf("exit\n");
			free_and_exit(g_status);
		}
		if (pipe_is_open(input)) // si ctrl-d pendant l'affichage de la nouvlelle ligne gérer la sortie du programme || remplacer if par while check_open_pipe
		{
			shell_prompt = "> ";
			continue ;
		}

		printf(RED "%s\n" RESET, input);
		if (!g_status && !ft_strequ(input, ""))
		{
			g_status = 0;
			add_history(input);
		}

		del_garbage();
		input = "";
		shell_prompt = create_prompt();
	}
}
