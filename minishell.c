/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:33:14 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/18 19:01:09 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define ANSI_COLOR_RED     "\x1b[31;1m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static uint8_t	g_status;

char	*create_prompt(void)
{
	char	*prompt;
	char	*home;

	home = getenv("HOME");
	prompt = getcwd(NULL, 0);//proteger retour NULL + ajouter une fonction add_ptr a notre garbage collector
	if (home && ft_strstr(prompt, home))
	{
		prompt[ft_strlen(home) - 1] = '~';
		prompt = &prompt[ft_strlen(home) - 1];
	}
	if (g_status)
		prompt = ft_strjoin("\x1b[31;1m➜ \x1b[36m", prompt);//proteger retour NULL
	else
		prompt = ft_strjoin("\x1b[32;1m➜ \x1b[36m", prompt);//proteger retour NULL
	prompt = ft_strjoin(prompt, "$ \x1b[0m");//proteger retour NULL
	return (prompt);
}

void	print_start(void)
{
	printf("\033[2J");
	printf("\033[100A");
	printf(" /$$      /$$ /$$$$$$ /$$   /$$ /$$$$$$");
	printf("  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$       /$$      \n");
	printf("| $$$    /$$$|_  $$_/| $$$ | $$|_  $$_/");
	printf(" /$$__  $$| $$  | $$| $$_____/| $$      | $$      \n");
	printf("| $$$$  /$$$$  | $$  | $$$$| $$  | $$  ");
	printf("| $$  \\__/| $$  | $$| $$      | $$      | $$      \n");
	printf("| $$ $$/$$ $$  | $$  | $$ $$ $$  | $$  ");
	printf("|  $$$$$$ | $$$$$$$$| $$$$$   | $$      | $$      \n");
	printf("| $$  $$$| $$  | $$  | $$  $$$$  | $$  ");
	printf(" \\____  $$| $$__  $$| $$__/   | $$      | $$      \n");
	printf("| $$\\  $ | $$  | $$  | $$\\  $$$  | $$  ");
	printf(" /$$  \\ $$| $$  | $$| $$      | $$      | $$      \n");
	printf("| $$ \\/  | $$ /$$$$$$| $$ \\  $$ /$$$$$$");
	printf("|  $$$$$$/| $$  | $$| $$$$$$$$| $$$$$$$$| $$$$$$$$\n");
	printf("|__/     |__/|______/|__/  \\__/|______/");
	printf(" \\______/ |__/  |__/|________/|________/|________/\n\n\n");
}

int	main(int argc, char *argv[], char **envp)
{
	int		i;
	char	*input;
	char	*shell_prompt;

	
	if (argc > 1)
		return (0);//ecrire error
	(void) argv; 
	(void) envp;
	i = 0;
	print_start();
	shell_prompt = create_prompt();
	while (i != 4)
	{

		input = readline(shell_prompt);
		if (!input)
			break ;
		add_history(input);
		printf(ANSI_COLOR_RED "%s\n" ANSI_COLOR_RESET, input);
		free(input);//ajouter input dans le garbage collectore + mem_remove de input
		i++;
	}
	del_garbage();
	rl_clear_history();
	return (0);
}
