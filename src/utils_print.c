/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:06:44 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/26 17:13:10 by rbicanic         ###   ########.fr       */
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
	// printf("%s", color);
	write(2, color, ft_strlen(RED));
	write(2, msg, ft_strlen(msg));
	// printf("\b%s\n" RESET, msg);
	if (exit_status)
		free_and_exit(exit_status);
}

uint8_t	print_first_check_error(char *msg, char *token, char **input,
		char **shell_prompt)
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

void	errno_file_error(char *file, int exit_status)
{
	write(2, RED, ft_strlen(RED));
	write(2, "Minishell: ", 11);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, ": ", 2);
	write(2, file, ft_strlen(file));
	write(2, RESET, ft_strlen(RESET));
	write(2, "\n", 1);
	free_and_exit(exit_status);
}