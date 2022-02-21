/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:06:44 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/21 16:10:51 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_start(void)
{
	printf("\033[2J");
	printf("\033[100A");
	printf("\n /$$      /$$ /$$$$$$ /$$   /$$ /$$$$$$");
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

void	print_message(char *msg, char *color, int exit_status)
{
	printf("%s", color);
	printf("%s", msg);
	printf(RESET);
	if (exit_status)
		free_and_exit(exit_status);
}

#define QUOTES_ERR "Minishell: open quotes are not interpreted.\n"

uint8_t	print_first_check_error(char *msg, char *token, char **input, char **shell_prompt)
{
	if (!token)
		printf(RED "%s" RESET, msg);
	else
		printf(RED "%s '%s'.\n" RESET, msg, token);
	*input = "";
	g_status = 1;
	*shell_prompt = create_prompt();
	return (1);
}
