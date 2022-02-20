/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 17:59:05 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/20 20:28:51 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <setjmp.h>
# include <errno.h>
# include <error.h>
# include <getopt.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdint.h>
# include "../libft/include/libft.h"

/*
**	Colors definition
*/

# define RED     "\x1b[31;1m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define RESET   "\x1b[0m"

/*
**	Defines
*/

# define DO_NOT_CLOSE_PROG 0

/*
**	Structures
*/

typedef struct s_pipe_command
{
	int		infile;
	int		outfile;
	char	*cmd_content;
	char	**exec_agrs;
}	t_pipe_command;

typedef struct s_command
{
	char	*control_op;
	char	*command;
	t_list	*command_list;
}	t_command;

/*
**	Global variables
*/

extern uint8_t	g_status;

/*
**	Terminal / print functions
*/

void	print_start(void);
char	*create_prompt(void);
void	prompt_loop(void);
uint8_t	pipe_is_open(char *str);
void	print_message(char *msg, char *color, int exit_status);

/*
**	Signals
*/

void	sig_handler(int sigcode);

/*
**	Exit
*/

void	free_and_exit(int exit_code);
void	child_error_exit(int status_error, char *file);
void	eof_exit(char *input);

/*
**	Error management
*/

# define PATH_CMD_ERR 127
# define PATH_FILE_ERR 127
# define RIGHT_FILE_ERR 126
# define EXEC_ERR 1
# define MALLOC_ERR 2
# define FD_ERR 1

# define ARGS_ERR_MSG "Error : Invalid number of arguments.\n"

uint8_t	near_unexpected_token_error(char **input, char **shell_prompt);
uint8_t open_quotes(char *input);
uint8_t	not_interpreted_characters(char *input);

/*
**	Parsing
*/

void	*single_pipe_parsing(t_list **command_list);
t_list	*parse_step1(char *input);

#endif
