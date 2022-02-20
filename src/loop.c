/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/20 20:34:32 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	prompt_loop(void)
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
