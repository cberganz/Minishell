/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:04:29 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/20 18:11:49 by rbicanic         ###   ########.fr       */
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
		prompt = ft_strjoin("\x1b[31;1m➜ \x1b[35m", prompt);
	else
		prompt = ft_strjoin("\x1b[32;1m➜ \x1b[35m", prompt);
	prompt = ft_strjoin(prompt, "$ \x1b[0m");
	if (!prompt)
		free_and_exit(MALLOC_ERR);
	return (prompt);
}

uint8_t	pipe_is_open(char *str)
{
	int	i;

	if (!str)
		return (0);
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
		input = ft_strjoin(input, garbage_addptr(readline(shell_prompt)));
		if (!input)
			eof_exit(input);//modif sur la fonction (input toujour null et check_open_pipe pas possible) => peut etre update error status auqnd open pipe
		if (near_unexpected_token_error(&input, &shell_prompt))
			continue ;
		if (pipe_is_open(input)) // si ctrl-d pendant l'affichage de la nouvelle ligne gérer la sortie du programme || si ctrl-c pas input pas reset + problem affichage ligne 
		{
			shell_prompt = "> ";
			continue ;
		}

		printf(GREEN "%s\n" RESET, input);
		if (!ft_strequ(input, ""))
		{
			g_status = 0;
			add_history(input);
		}

		del_garbage();
		input = "";
		shell_prompt = create_prompt();
	}
}
