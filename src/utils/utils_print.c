/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:06:44 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/19 14:34:10 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_start(void)
{
	ft_putstr_fd("\033[2J", 2);
	ft_putstr_fd("\033[100A", 2);
	ft_putstr_fd("\n /$$      /$$ /$$$$$$ /$$   /$$ /$$$$$$", 2);
	ft_putstr_fd("  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$       /$$      \n", 2);
	ft_putstr_fd("| $$$    /$$$|_  $$_/| $$$ | $$|_  $$_/", 2);
	ft_putstr_fd(" /$$__  $$| $$  | $$| $$_____/| $$      | $$      \n", 2);
	ft_putstr_fd("| $$$$  /$$$$  | $$  | $$$$| $$  | $$  ", 2);
	ft_putstr_fd("| $$  \\__/| $$  | $$| $$      | $$      | $$      \n", 2);
	ft_putstr_fd("| $$ $$/$$ $$  | $$  | $$ $$ $$  | $$  ", 2);
	ft_putstr_fd("|  $$$$$$ | $$$$$$$$| $$$$$   | $$      | $$      \n", 2);
	ft_putstr_fd("| $$  $$$| $$  | $$  | $$  $$$$  | $$  ", 2);
	ft_putstr_fd(" \\____  $$| $$__  $$| $$__/   | $$      | $$      \n", 2);
	ft_putstr_fd("| $$\\  $ | $$  | $$  | $$\\  $$$  | $$  ", 2);
	ft_putstr_fd(" /$$  \\ $$| $$  | $$| $$      | $$      | $$      \n", 2);
	ft_putstr_fd("| $$ \\/  | $$ /$$$$$$| $$ \\  $$ /$$$$$$", 2);
	ft_putstr_fd("|  $$$$$$/| $$  | $$| $$$$$$$$| $$$$$$$$| $$$$$$$$\n", 2);
	ft_putstr_fd("|__/     |__/|______/|__/  \\__/|______/", 2);
	ft_putstr_fd(" \\______/ |__/  |__/|________/|________/|________/\n\n\n", 2);
}

void	print_message(char *msg, char *color, int exit_status)
{
	// printf("%s", color);
	write(2, color, ft_strlen(color));
	write(2, msg, ft_strlen(msg));
	write(2, RESET, ft_strlen(RESET));
	// printf("\b%s\n" RESET, msg);
	if (exit_status)
		free_and_exit(exit_status);
}

uint8_t	print_first_check_error(char *msg, char *token, char **input,
		char **shell_prompt)
{
	if (!token)
	{
		print_message(msg, RED, 0);// modif ici
		print_message("\n", RED, 0);// modif ici
	}
	else
	{
		print_message(msg, RED, 0);//modif ici
		print_message(" '", RED, 0);//modif ici
		print_message(token, RED, 0);//modif ici
		print_message("'.\n", RED, 0);//modif ici
		// printf(RED "%s '%s'.\n" RESET, msg, token);
	}
	*input = "";
	g_status = 2;
//	*shell_prompt = create_prompt(); A REMETRE
	(void)shell_prompt;
	return (1);
}

void	errno_file_error(char *file, int exit_status)
{
	write(2, RED, ft_strlen(RED));
	write(2, "Minishell: ", 11);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, ": ", 2);
	write(2, file, ft_strlen(file));

	// print_message("\n", RED, exit_status);
	write(2, RESET, ft_strlen(RESET));
	write(2, "\n", 1);
	g_status = exit_status;// check si >wejbr |<wernq renvoie 0
	// if (exit_status)
	// 	free_and_exit(exit_status);
}

int	print_dirpath_err(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (access(path, F_OK) == -1)
		ft_putstr_fd("no such file or directory: ", 2);
	else if (access(path, R_OK) == -1)
		ft_putstr_fd("permission denied: ", 2);
	else
		ft_putstr_fd("not a directory: ", 2);
	ft_putendl_fd(path, 2);
	return (1);
}
